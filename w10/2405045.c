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

int documents_count[MAX_DOCS];
int document_tokens_idx[MAX_DOCS];

int token_count, docs_count;

void
normalize_case_all ()
{
  if (docs_count == 0)
    printf ("No documents set. Use 'set' command first.\n");
  else
    for (int i = 0; i < MAX_DOCS; i++)
      {
        if (documents[i][0] == 0)
          break;
        for (int j = 0; j < MAX_LEN; j++)
          documents[i][j] = tolower (documents[i][j]);
        printf ("Document %d: %s\n", i + 1, documents[i]);
      }
}

void
tokenize_all ()
{
  for (int i = 0; i < MAX_DOCS; i++)
    document_tokens_idx[i] = -1;
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
                tokens[k][l] = 0, k++, l = 0;
              tokens[k][l++] = documents[i][j];
              w = 1;
            }
          else
            w = 0;
        }
      k++, l = 0;
      if (i == 0)
        documents_count[i] = k;
      else
        documents_count[i] = k - document_tokens_idx[i - 1] - 1;
      document_tokens_idx[i] = k - 1;
      if (k == MAX_TOKENS)
        break;
    }
  token_count = k;
  for (i = 0; i < token_count; i++)
    printf ("%d. %s\n", i + 1, tokens[i]);
  for (i = 0; i < docs_count; i++)
    printf ("Document %d: %d %d\n", i + 1, documents_count[i],
            document_tokens_idx[i]);
}

void
remove_stop_words_all ()
{
  if (token_count == 0)
    printf ("No tokens available. Use 'tokenize' command first.\n");
  int i, j, k = 0, l, m;
  for (i = 0; i < token_count; i++)
    {
      for (j = 0; j < NUM_STOP_WORDS; j++)
        if (!strcmp (tokens[i], stop_words[j]))
          break;
      if (j >= NUM_STOP_WORDS)
        strcpy (tokens_except_stop_words[k++], tokens[i]);
      else
        {
          if (i<=document_tokens_idx[0]) l=-1;
          else 
          for (l = 0; l < docs_count; l++)
            {
              if (i > document_tokens_idx[l])
                break;
            }
          documents_count[l + 1]--;
          for (m = l + 1; m < MAX_DOCS; m++)
            document_tokens_idx[m]--;
        }
      tokens[i][0] = 0;
    }
  token_count = k;
  for (i = 0; i < token_count; i++)
    strcpy (tokens[i], tokens_except_stop_words[i]),
        printf ("%d. %s\n", i + 1, tokens[i]);
  for (i = 0; i < docs_count; i++)
    printf ("Document %d: %d %d\n", i + 1, documents_count[i],
            document_tokens_idx[i]);
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
          char *p = tokens[i];
          size_t len = strlen (tokens[i]);
          size_t slen = strlen (suffixes[j]);
          if (len >= slen && strcmp (tokens[i] + len - slen, suffixes[j]) == 0)
            {
              tokens[i][len - slen] = '\0';
              d = 1;
            }
        }
      strncpy (stemmed_tokens[i], tokens[i], MAX_TOKEN_LEN);
      printf ("%d. %s\n", i + 1, tokens[i]);
    }
}

double
compute_tf (char word[], int doc_id)
{
  int i, n = 0;
  double tf;
  if (doc_id == 0)
    i = 0;
  else
    i = document_tokens_idx[doc_id - 1];
  for (; i <= document_tokens_idx[doc_id]; i++)
    {
      if (!strncmp (stemmed_tokens[i], word, strlen (word)))
        n++;
    }
  tf = 1. * n / documents_count[doc_id];
  return tf;
}

double
compute_idf (char word[])
{
  int i, j = 0, n = 0;
  for (i = 0; i < token_count; i++)
    {
      if (!strncmp (word, tokens[i], strlen (word)))
        n++, i = document_tokens_idx[++j] + 1;
    }
  double idf = log10 (1. * MAX_DOCS / (1 + n));
  return idf;
}

void
compute_tfidf_all (char word[])
{
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
}

void
display_stat ()
{
}

int
main ()
{
  for (int i = 0; i < MAX_DOCS; i++)
    documents[i][0] = '\0';

  char cmd[21], doc_str[MAX_LEN + 2], word[MAX_TOKEN_LEN];

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
            printf ("Documents set successfully\n");
          else
            printf ("No valid documents were given.\n");
        }
      else if (!strcmp ("normalize", cmd))
        normalize_case_all ();
      else if (!strcmp ("tokenize", cmd))
        tokenize_all ();
      else if (!strcmp ("remove_stop", cmd))
        remove_stop_words_all ();
      else if (!strcmp ("stem", cmd))
        stem_all_tokens ();
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
              printf ("Enter word to compute TF: ");
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
              printf ("Enter word to compute TF: ");
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
        exit (0);
      else
        printf ("Unknown command. Type 'help' for list of commands.\n");
    }
}
