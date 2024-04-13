#include <stdio.h>
#include <curl/curl.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/xpathInternals.h>
#include <stdlib.h>
#include <memory.h>

#define XML "https://www.mgm.gov.tr/FTPDATA/analiz/sonSOA.xml"

struct Memstr {
    char *memory;
    size_t size;
};

static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct Memstr *mem = (struct Memstr *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if(ptr == NULL) {
        printf("yetersiz bellek (NOT ENOUGH ALLOCATION)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&mem->memory[mem->size], contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

void parsexml(char *XmlContent) {
    xmlDocPtr doc;
    xmlNodePtr cur;
    xmlXPathContextPtr xpathCtx;
    xmlXPathObjectPtr xpathObj;

    doc = xmlParseDoc((xmlChar * ) XmlContent);
    if(doc == NULL) {
        fprintf(stderr, "dokuman ayirma basarisiz \n");
        return;
    }

    cur = xmlDocGetRootElement(doc);
    if(cur == NULL) {
        fprintf(stderr, "bos dokuman MGM yanit yok\n");
        xmlFreeDoc(doc);
        return;
    }

    xpathCtx = xmlXPathNewContext(doc);
    if(xpathCtx == NULL) {
        fprintf(stderr,"Error: unable to create new XPath context\n");
        xmlFreeDoc(doc);
        return;
    }

    xpathObj = xmlXPathEvalExpression((xmlChar*)"//sehirler[ili='İSTANBUL']/Mak", xpathCtx);    if(xpathObj == NULL) {
        fprintf(stderr,"Error: unable to evaluate xpath expression\n");
        xmlXPathFreeContext(xpathCtx);
        xmlFreeDoc(doc);
        return;
    }

    xmlNodeSetPtr nodeset = xpathObj->nodesetval;
    if (nodeset == NULL) {
        fprintf(stderr, "Error: no nodes matched the XPath expression\n");
    } else if (nodeset->nodeNr > 0) {
        xmlChar* keyword = xmlNodeListGetString(doc, nodeset->nodeTab[0]->xmlChildrenNode, 1);
        printf("Max temperature in Istanbul: %s\n", keyword);
        xmlFree(keyword);
    } else {
        fprintf(stderr, "Error: no nodes matched the XPath expression\n");
    }

    xmlXPathFreeObject(xpathObj);
    xmlXPathFreeContext(xpathCtx);
    xmlFreeDoc(doc);
}


int main(void) {
      CURL *curl_handle;
    CURLcode res;

    struct Memstr chunk;

    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_ALL);

    curl_handle = curl_easy_init();

    curl_easy_setopt(curl_handle, CURLOPT_URL, XML);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)&chunk);
    curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");

    res = curl_easy_perform(curl_handle);

    if(res != CURLE_OK) {
        fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        printf("%lu bytes retrieved\n", (unsigned long)chunk.size);
        parsexml(chunk.memory);
    }

    curl_easy_cleanup(curl_handle);
    free(chunk.memory);
    curl_global_cleanup();

    return 0;
}

