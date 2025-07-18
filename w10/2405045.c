#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DOCS 50
#define MAX_LEN 5000
#define MAX_TOKENS 500
#define MAX_TOKEN_LEN 50

#define NUM_STOP_WORDS 8

char documents[MAX_DOCS][MAX_LEN];
char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
char tokens_except_stop_words[MAX_TOKENS][MAX_TOKEN_LEN];
char stemmed_tokens[MAX_TOKENS][MAX_TOKEN_LEN];

char stop_words[NUM_STOP_WORDS][MAX_TOKEN_LEN]
    = { "the", "is", "a", "an", "and", "in", "of", "to" };
char suffixes[3][4] = { "ing", "ed", "s" };

int tokens_doc_id[MAX_TOKENS];
int tokens_except_stop_words_doc_id[MAX_TOKENS];

int token_count, docs_count;

void
normalize_case_all ()
{
  if (docs_count == 0)
    printf ("No documents set. Use 'set' command first.\n");
  else
    {
      printf ("Normalized Documents: \n");
      for (int i = 0; i < MAX_DOCS; i++)
        {
          if (documents[i][0] == 0)
            break;
          for (int j = 0; j < MAX_LEN; j++)
            documents[i][j] = tolower (documents[i][j]);
          printf ("Document %d: %s\n", i + 1, documents[i]);
        }
    }
  printf ("Documents normalized.\n");
}

void
tokenize_all ()
{
  for (int i = 0; i < MAX_TOKENS; i++)
    tokens[i][0] = 0;

  if (docs_count == 0)
    printf ("No documents set. Use 'set' command first\n");

  int i, j, k, l, w = 0;
  for (i = j = k = l = 0; i < docs_count; i++)
    {
      for (j = 0; documents[i][j]; j++)
        {
          if (k == MAX_TOKENS)
            break;
          if (isalnum (documents[i][j]))
            {
              if ((j != 0 && !w) || l >= MAX_TOKEN_LEN - 1)
                tokens[k][l] = 0, k++, l = 0, tokens_doc_id[k - 1] = i;
              tokens[k][l++] = documents[i][j];
              w = 1;
            }
          else
            w = 0;
        }
      k++, l = 0, tokens_doc_id[k - 1] = i;
      if (k == MAX_TOKENS)
        break;
    }
  token_count = k;
  printf ("Tokens: \n");
  for (i = 0; i < token_count; i++)
    printf ("%d. %s\n", i + 1, tokens[i]);
  printf ("Tokenization complete. Total tokens: %d\n", token_count);
}

void
remove_stop_words_all ()
{
  if (token_count == 0)
    printf ("No tokens available. Use 'tokenize' command first.\n");
  int i, j, k = 0;
  for (i = 0; i < token_count; i++)
    {
      for (j = 0; j < NUM_STOP_WORDS; j++)
        if (!strcmp (tokens[i], stop_words[j]))
          break;
      if (j >= NUM_STOP_WORDS)
        tokens_except_stop_words_doc_id[k] = tokens_doc_id[i],
        strcpy (tokens_except_stop_words[k++], tokens[i]);
      tokens[i][0] = 0;
    }
  token_count = k;
  printf ("Tokens after stop-word removal: \n");
  for (i = 0; i < token_count; i++)
    strcpy (tokens[i], tokens_except_stop_words[i]),
        tokens_doc_id[i] = tokens_except_stop_words_doc_id[i],
        printf ("%d. %s\n", i + 1, tokens[i]);
  printf ("Stop-word removal complete. Tokens remaining: %d\n", token_count);
}

void
stem_all_tokens ()
{
  if (token_count == 0)
    {
      printf ("No tokens available. Use 'tokenize' command first.\n");
      return;
    }
  int i, j, d;
  for (i = 0; i < token_count; i++)
    {
      d = 0;
      for (j = 0; !d && j < 3; j++)
        {
          int len = strlen (tokens[i]);
          int slen = strlen (suffixes[j]);
          if (len >= slen && strcmp (tokens[i] + len - slen, suffixes[j]) == 0)
            {
              tokens[i][len - slen] = '\0';
              d = 1;
            }
        }
      strncpy (stemmed_tokens[i], tokens[i], MAX_TOKEN_LEN);
    }
  printf ("Stemmed Tokens: \n");
  for (int i = 0; i < token_count; i++)
    printf ("%d. %s\n", i + 1, tokens[i]);
  printf ("Stemming complete. Total stemmed tokens: %d\n", token_count);
}

double
compute_tf (char word[], int doc_id)
{
  int i, n = 0, m = 0;
  double tf;
  for (i = 0; tokens_doc_id[i] != doc_id; i++)
    ;
  for (; tokens_doc_id[i] == doc_id && i < token_count; n++, i++)
    {
      if (!strncmp (tokens[i], word, MAX_TOKEN_LEN))
        m++;
    }
  tf = 1. * m / n;
  return tf;
}

double
compute_idf (char word[])
{
  int i, j = tokens_doc_id[0], m = 0;
  for (i = 0; i < token_count; i++)
    {
      if (!strcmp (word, tokens[i]))
        {
          m++;
          for (; tokens_doc_id[i] == j && j < docs_count && i < token_count;
               i++)
            ;
          j = tokens_doc_id[i];
        }
    }
  double idf = log10 (1. * MAX_DOCS / (1 + m));
  return idf;
}

void
compute_tfidf_all (char word[])
{
  printf ("TF-IDF for '%s': \n", word);
  double idf = compute_idf (word);
  for (int i = 0; i < docs_count; i++)
    {
      double tf = compute_tf (word, i);
      printf ("Document %d: %.4f\n", i + 1, tf * idf);
    }
}

void
help ()
{
  printf (
      "set         - Prompt for the number of documents and their text.\n");
  printf ("preprocess  - Apply normalization, tokenization, stop-words "
          "removal, and stemming orderly.\n");
  printf ("tf          - Compute and display Term Frequency for a specified "
          "word across documents.\n");
  printf ("idf         - Compute and display Inverse Document Frequency for a "
          "specified word.\n");
  printf ("tfidf       - Compute and display TF-IDF scores for a specified "
          "word across documents.\n");
  printf ("stat        - Display TF, IDF, and TF-IDF for all tokens across "
          "all documents in a matrix format.\n");
  printf ("help        - Print the list of all available commands.\n");
  printf ("exit        - Exit the program.\n");
}

int
cmp_lexico (const void *a, const void *b)
{
  return strcmp ((char *)a, (char *)b);
}

void
display_stat ()
{
  if (docs_count == 0)
    printf ("No documents set. Use ‘set’ command first.");
  if (token_count == 0 && docs_count != 0)
    printf ("Documents set successfully. Please, enter ‘preprocess’ command "
            "now. It will not take other commands.\n");
  char sorted_tokens[token_count][MAX_TOKEN_LEN];
  for (int i = 0; i < token_count; i++)
    strncpy (sorted_tokens[i], tokens[i], MAX_TOKEN_LEN);
  for (int i = 0; i < token_count - 1; i++)
    {
      for (int j = i + 1; j < token_count; j++)
        {
          if (!strcmp (sorted_tokens[i], sorted_tokens[j]))
            sorted_tokens[j][0] = 0;
        }
    }
  qsort ((void *)sorted_tokens, token_count, MAX_TOKEN_LEN * sizeof (char),
         cmp_lexico);

  printf ("============== TF ================\n");
  printf ("%-50s", "");
  for (int i = 0; i < docs_count; i++)
    printf ("%9s%d", "doc", i + 1);
  printf ("\n");
  for (int i = 0; i < token_count; i++)
    {
      if (sorted_tokens[i][0] == 0)
        continue;
      printf ("%-50s", sorted_tokens[i]);
      for (int j = 0; j < docs_count; j++)
        {
          printf ("%10.4f", compute_tf (sorted_tokens[i], j));
        }
      printf ("\n");
    }
  printf ("\n================ IDF ================\n");
  printf ("%-50s", "");
  printf ("%6s", "IDF");
  printf ("\n");
  for (int i = 0; i < token_count; i++)
    {
      if (sorted_tokens[i][0] == 0)
        continue;
      printf ("%-50s", sorted_tokens[i]);
      printf ("%6.4f", compute_idf (sorted_tokens[i]));
      printf ("\n");
    }
  printf ("\n============== TF-IDF ================\n");
  printf ("%-50s", "");
  for (int i = 0; i < docs_count; i++)
    printf ("%9s%d", "doc", i + 1);
  printf ("\n");
  for (int i = 0; i < token_count; i++)
    {
      if (sorted_tokens[i][0] == 0)
        continue;
      printf ("%-50s", sorted_tokens[i]);
      double idf = compute_idf (sorted_tokens[i]);
      for (int j = 0; j < docs_count; j++)
        {
          printf ("%10.4f", compute_tf (sorted_tokens[i], j) * idf);
        }
      printf ("\n");
    }
}

int
main ()
{
  for (int i = 0; i < MAX_DOCS; i++)
    documents[i][0] = '\0';

  char cmd[21], doc_str[MAX_LEN + 2], word[MAX_TOKEN_LEN];

  printf ("Welcome to the Document Processing System!\n");
  help ();
  for (;;)
    {
      printf ("Enter command: ");
      fgets (cmd, 20, stdin);
      cmd[strlen (cmd) - 1] = 0;
      if (!strcmp ("set", cmd))
        {
          while (1)
            {
              printf ("Enter number of documents (1-50): ");
              scanf ("%d%*c", &docs_count);
              if (!(docs_count >= 1 && docs_count <= 50))
                printf (
                    "Invalid number of documents. Must be from 1 to 50.\n"),
                    docs_count = 0;
              else
                break;
            }
          int i;
          for (i = 0; i < MAX_DOCS; i++)
            documents[i][0] = '\0';
          for (i = 0; i < docs_count; i++)
            {
              printf ("Enter document %d: ", i + 1);
              fgets (doc_str, MAX_LEN + 1, stdin);
              if (strlen (doc_str) >= MAX_LEN
                  || doc_str[strlen (doc_str) - 1] != '\n')
                printf ("Document too long.\n"), i--;
              else
                {
                  if (doc_str[strlen (doc_str) - 1] == '\n')
                    doc_str[strlen (doc_str) - 1] = 0;
                  strncpy (documents[i], doc_str, MAX_LEN);
                }
            }
          if (i > 0)
            printf ("Documents set successfully. Please, enter `preprocess' "
                    "command now. It will not take other commands.\n");
          else
            printf ("No valid documents were given.\n");
        }
      else if (!strcmp ("preprocess", cmd))
        {
          normalize_case_all ();
          tokenize_all ();
          remove_stop_words_all ();
          stem_all_tokens ();
        }
      else if (!strcmp ("tf", cmd))
        {
          while (1)
            {
              printf ("Enter word to compute TF: ");
              fgets (word, MAX_TOKEN_LEN, stdin);
              if (word[strlen (word) - 1] == '\n')
                word[strlen (word) - 1] = 0;
              if (!word[0])
                printf ("Invalid word. Must be a valid word.\n");
              else
                break;
            }
          for (int i = 0; word[i]; word[i] = tolower (word[i]), i++)
            ;
          for (int i = 0; i < docs_count; i++)
            {
              printf ("Document %d: %.4f\n", i + 1, compute_tf (word, i));
            }
        }
      else if (!strcmp ("idf", cmd))
        {
          while (1)
            {
              printf ("Enter word to compute IDF: ");
              fgets (word, MAX_TOKEN_LEN, stdin);
              word[strlen (word) - 1] = 0;
              if (!word[0])
                printf ("Invalid word. Must be a valid word.\n");
              else
                break;
            }
          printf ("IDF for '%s': %.4f\n", word, compute_idf (word));
        }
      else if (!strcmp ("tfidf", cmd))
        {
          while (1)
            {
              printf ("Enter word to compute TF-IDF: ");
              fgets (word, MAX_TOKEN_LEN, stdin);
              word[strlen (word) - 1] = 0;
              if (!word[0])
                printf ("Invalid word. Must be a valid word.\n");
              else
                break;
            }
          compute_tfidf_all (word);
        }
      else if (!strcmp ("help", cmd))
        help ();
      else if (!strcmp ("stat", cmd))
        display_stat ();
      else if (!strcmp ("exit", cmd))
        printf ("Exiting program.\n"), exit (0);
      else
        printf ("Unknown command. Type 'help' for list of commands.\n");
    }
}
