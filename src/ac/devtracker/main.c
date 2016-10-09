#include <strings.h>
#include <stdio.h>
#include <curl/curl.h>
#include <tidy/tidy.h>
#include <tidy/buffio.h>

// Henry Brown
// 2015
//
// Largely sourced from http://curl.haxx.se/libcurl/c/getinmemory.html

struct MemoryStruct {
  char* memory;
  size_t size;
};

struct DevTracker {
  char* title;
  char* url;
  char* date;
  char* time;
  char* forum;
};

struct DevTracker dev_tracker_entries[20];

int skipFirst = 1;
int processed_dev_tracker_rows = 0;

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb,
    void* userp);

int process_as_dom();
void process_dom(TidyDoc doc, TidyNode tnod);
void process_dev_tracker_row(TidyDoc doc, TidyNode tnod);
void process_thread_title(TidyDoc doc, TidyNode tnod);
void process_thread_status(TidyDoc doc, TidyNode tnod);
void process_thread_forum(TidyDoc doc, TidyNode tnod);
int process_as_string();
void dumpNode(TidyDoc doc, TidyNode tnod, int indent);
uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out);

int main() {
  return process_as_dom();
}

int process_as_dom() {
  char* dev_tracker = "https://www.asheronscall.com/en/forums/" \
    "post_tracker.php?tracker=devtracker";
  char* user_agent = "libcurl-agent/1.0";

  CURL *curl;

  char curl_errbuf[CURL_ERROR_SIZE];

  TidyDoc tdoc;
  TidyBuffer docbuf = {0};
  TidyBuffer tidy_errbuf = {0};

  int err;
  int i;

  curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, dev_tracker);
  curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_errbuf);
  curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
  curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L);
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_cb);
  curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent);

  tdoc = tidyCreate();
  tidyOptSetBool(tdoc, TidyForceOutput, yes);
  tidyOptSetInt(tdoc, TidyWrapLen, 4096);
  tidySetErrorBuffer(tdoc, &tidy_errbuf);
  tidyBufInit(&docbuf);

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, &docbuf);

  err = curl_easy_perform(curl);

  if (!err) {
    err = tidyParseBuffer(tdoc, &docbuf);
    if (err >= 0) {
      err = tidyCleanAndRepair(tdoc);

      if (err >= 0) {
        err = tidyRunDiagnostics(tdoc);

        if (err >= 0) {
          skipFirst = 1;
          process_dom(tdoc, tidyGetRoot(tdoc));
        }
      }
    }
  } else {
    fprintf(stderr, "%s\n", curl_errbuf);
  }

  curl_easy_cleanup(curl);
  tidyBufFree(&docbuf);
  tidyBufFree(&tidy_errbuf);
  tidyRelease(tdoc);

  struct DevTracker* entry;

  for (i = 0; i < 20; ++i) {
    entry = &dev_tracker_entries[i];

    fprintf(stderr, "%s URL: %s\n%s: %s %s\n", entry->title, entry->url,
        entry->forum, entry->date, entry->time);
  }

    /*
struct DevTracker {
  char* title;
  char* url;
  char* date;
  char* time;
  char* forum;
};

struct DevTracker dev_tracker_entries[20];
*/

  return err;
}

void process_dom(TidyDoc doc, TidyNode tnod) {
  TidyNode child;

  if (processed_dev_tracker_rows >= 20) {
    return;
  }

  for (child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
    ctmbstr name = tidyNodeGetName(child);

    if (name) {
      TidyAttr attr;

      for (attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr)) {
        if (!strcmp(tidyAttrName(attr), "class") &&
            !strcmp(tidyAttrValue(attr), "dev-tracker-row")) {
          if (!skipFirst) {
            process_dev_tracker_row(doc, child);

            ++processed_dev_tracker_rows;
          } else {
            skipFirst = 0;
          }
        }
      }
    }


    if (processed_dev_tracker_rows >= 20) {
      return;
    }

    process_dom(doc, child);
  }
}

void process_dev_tracker_row(TidyDoc doc, TidyNode tnod) {
  TidyNode child;

  for (child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
    ctmbstr name = tidyNodeGetName(child);

    if (name) {
      TidyAttr attr;

      for (attr = tidyAttrFirst(child); attr; attr=tidyAttrNext(attr)) {
        if (!strcmp(tidyAttrName(attr), "class") &&
            !strcmp(tidyAttrValue(attr), "dev-tracker-span6 threadtitle")) {
          process_thread_title(doc, child);
        } else if
           (!strcmp(tidyAttrName(attr), "class") &&
            !strcmp(tidyAttrValue(attr), "dev-tracker-span3 threadstatus")) {
          process_thread_status(doc, child);
        } else if
           (!strcmp(tidyAttrName(attr), "class") &&
            !strcmp(tidyAttrValue(attr), "dev-tracker-span3 threadforum")) {
          process_thread_forum(doc, child);
        }
      }
    }
  }
}

void process_thread_title(TidyDoc doc, TidyNode tnod) {
  TidyNode child;
  TidyNode title_child;

  TidyAttr attr;

  for (child = tidyGetChild(tnod); tidyGetNext(child); child = tidyGetNext(child)) {
  }

  struct DevTracker* entry = &dev_tracker_entries[processed_dev_tracker_rows];

  for (attr = tidyAttrFirst(child); attr; attr=tidyAttrNext(attr)) {
    if (!strcmp(tidyAttrName(attr), "href")) {
      entry->url = malloc(strlen(tidyAttrValue(attr)));

      strcpy(entry->url, (char*) tidyAttrValue(attr));

      entry->url[strlen(tidyAttrValue(attr)) - 1] = '\0';

      break;
    }
  }

  title_child = tidyGetChild(child);

  TidyBuffer buf;
  tidyBufInit(&buf);
  tidyNodeGetText(doc, title_child, &buf);

  entry->title = 0;

  if (buf.bp) {
    entry->title = malloc(buf.size);

    strcpy(entry->title, (char*) buf.bp);

    entry->title[buf.size - 1] = '\0';
  }

  tidyBufFree(&buf);
}

void process_thread_status(TidyDoc doc, TidyNode tnod) {
  TidyNode child;
  TidyNode time_child;

  child = tidyGetChild(tnod);

  TidyBuffer buf;
  TidyBuffer buf2;
  tidyBufInit(&buf);
  tidyBufInit(&buf2);

  tidyNodeGetText(doc, child, &buf);

  struct DevTracker* entry = &dev_tracker_entries[processed_dev_tracker_rows];

  entry->date = 0;
  entry->time = 0;

  if (buf.bp) {
    entry->date = malloc(buf.size);

    strcpy(entry->date, (char*) buf.bp);

    entry->date[buf.size - 1] = '\0';
  }

  tidyBufFree(&buf);

  child = tidyGetNext(child);

  time_child = tidyGetChild(child);

  tidyNodeGetText(doc, time_child, &buf2);

  if (buf2.bp) {
    entry->time = malloc(buf2.size);

    strcpy(entry->time, (char*) buf2.bp);

    entry->time[buf2.size - 1] = '\0';
  }

  tidyBufFree(&buf);
}

void process_thread_forum(TidyDoc doc, TidyNode tnod) {
  TidyNode child;
  TidyNode forum_child;

  child = tidyGetChild(tnod);

  forum_child = tidyGetChild(child);

  TidyBuffer buf;
  tidyBufInit(&buf);
  tidyNodeGetText(doc, forum_child, &buf);

  struct DevTracker* entry = &dev_tracker_entries[processed_dev_tracker_rows];

  entry->forum = 0;

  if (buf.bp) {
    entry->forum = malloc(buf.size);

    strcpy(entry->forum, (char*) buf.bp);

    entry->forum[buf.size - 1] = '\0';
  }

  tidyBufFree(&buf);
}

int process_as_string() {
  char* dev_tracker = "https://www.asheronscall.com/en/forums/" \
    "post_tracker.php?tracker=devtracker";
  char* user_agent = "libcurl-agent/1.0";

  CURL *curl_handle;
  CURLcode res;

  struct MemoryStruct chunk;

  chunk.memory = malloc(1);
  chunk.size   = 0;

  curl_global_init(CURL_GLOBAL_ALL);

  curl_handle = curl_easy_init();

  curl_easy_setopt(curl_handle, CURLOPT_URL, dev_tracker);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

  curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *) &chunk);

  curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, user_agent);

  res = curl_easy_perform(curl_handle);

  if (res != CURLE_OK) {
    fprintf(stderr, "curl_easy_perform() failed: %s\n",
        curl_easy_strerror(res));

    return 1;
  } else {
    fwrite(chunk.memory, sizeof(char), chunk.size, stderr);
  }

  curl_easy_cleanup(curl_handle);

  free(chunk.memory);

  curl_global_cleanup();

  return 0;
}

static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb,
    void* userp) {
  size_t realsize = size * nmemb;

  struct MemoryStruct *mem = (struct MemoryStruct *) userp;

  mem->memory = realloc(mem->memory, mem->size + realsize + 1);

  if (mem->memory == NULL) {
    fprintf(stderr, "Not enough memory (realloc returned NULL)\n");

    return 0;
  }

  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;

  return realsize;
}

void dumpNode(TidyDoc doc, TidyNode tnod, int indent) {
  TidyNode child;

  for (child = tidyGetChild(tnod); child; child = tidyGetNext(child)) {
    ctmbstr name = tidyNodeGetName(child);

    if (name) {
      TidyAttr attr;

      printf(" %*.*s%s ", indent, indent, "<", name);

      for (attr=tidyAttrFirst(child); attr; attr=tidyAttrNext(attr)) {
        printf(tidyAttrName(attr));
        if (tidyAttrValue(attr)) {
          printf("=\"%s\" ", tidyAttrValue(attr));
        } else {
          printf(" ");
        }
      }

      printf(">\n");
    } else {
      TidyBuffer buf;
      tidyBufInit(&buf);
      tidyNodeGetText(doc, child, &buf);
      printf("%*.*s\n", indent, indent,
          (buf.bp ? (char*) buf.bp : ""));
      tidyBufFree(&buf);
    }
    dumpNode(doc, child, indent + 4);
  }
}

uint write_cb(char *in, uint size, uint nmemb, TidyBuffer *out) {
  uint r;
  r = size * nmemb;
  tidyBufAppend(out, in, r);
  return r;
}
