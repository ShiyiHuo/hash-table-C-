#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "string_set.h"

using namespace std;

string_set::string_set() {
    //initialize storage
    for (int i=0; i<HASH_TABLE_SIZE; i++) {
        hash_table[i]=NULL;
    }
    //initialize variables
    printf("test");
    reset();
}

void string_set::add(const char *s) {
    //find the linked list
    int index=hash_function(s);
    
    if (contains(s)==1) {
        throw duplicate_exception();
    }
    
    node *pointer=new node;
    if (pointer==NULL) {
        throw memory_exception();
    }
    pointer->s=new char[strlen(s)+1];
    /*
    if (pointer->s==NULL) {
        throw memory_exception();
    }
    */
    strcpy(pointer->s,s);
    
    //link in
    pointer->next=hash_table[index];
    hash_table[index]=pointer;
    
    //reset iterator
    reset();
}

void string_set::remove(const char *s) {
    int index=hash_function(s);
    
    node *curr=hash_table[index];
    node *prev=NULL;
        
    while (curr!=NULL) {
        if (strcmp(s,curr->s)!=0) {
            prev=curr;
            curr=curr->next;
        } else {
            break;
        }
    }
    if (curr==NULL) {
        throw not_found_exception();
    }
    if (prev==NULL) {
        hash_table[index]=curr->next;
    } else {
        prev->next=curr->next;
        delete curr->s;
        delete curr;
    }
    
    //reset iterator
    reset();
}

int string_set::contains(const char *s) {
    int index=hash_function(s);
    
    for (node *pointer=hash_table[index]; pointer!=NULL; pointer=pointer->next) {
        if (strcmp(s,pointer->s)==0) {
            return 1;
        }
    }
    return 0;
}

void string_set::reset() {
    iterator_index=0;
    iterator_node=hash_table[0];
}

const char *string_set::next() {
    while (iterator_index<HASH_TABLE_SIZE) {
        while (iterator_node!=NULL) {
            char *result=iterator_node->s;
            iterator_node=iterator_node->next;
            return result;
        }
        //iterator_index++;
        iterator_node=hash_table[++iterator_index];
    }
    return NULL;
    
}

//delete all dynamic storage allocated for nodes and strings
string_set::~string_set() {
    for (int i=0; i<HASH_TABLE_SIZE; i++) {
        node *ptr_node_to_be_deleted=hash_table[i];
        while (ptr_node_to_be_deleted != NULL) {
            node *temp=ptr_node_to_be_deleted->next;
            delete[] ptr_node_to_be_deleted->s;
            delete ptr_node_to_be_deleted;
            ptr_node_to_be_deleted=temp;
        }
        hash_table[i]=NULL;
    }
    reset();
}

int string_set::hash_function(const char *s) {
    int sum=0;
    for (int i=0; i<strlen(s); i++) {
        sum+=(int)s[i];
    }
    int hash_value=sum % HASH_TABLE_SIZE;
    return hash_value;
}
