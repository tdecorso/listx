#define LIST_IMPLEMENTATION
#include "list.h"
#include "test.h"
#include <stdint.h>

#define MAX_NODES 10000

int main(void) {
    list_t* l;
    TEST_SECTION("List initialization") {
        l = list_alloc(sizeof(int), MAX_NODES);

        if (!l) exit(EXIT_FAILURE);

        CHECK_BOOL(
            l->head == NULL,
            "Head is null at init.",
            ""
        );

        CHECK_BOOL(
            l->tail == NULL,
            "Tail is null at init.",
            ""
        );

        CHECK_BOOL(
            l->count == 0,
            "Count is 0 at init.",
            ""
        );

        CHECK_BOOL(
            l->elem_size == sizeof(int),
            "Elem_size matches given input.",
            ""
        );

        list_free(l);
    };

    TEST_SECTION("List front insert on empty list") {

        l = list_alloc(sizeof(int), MAX_NODES);

        int* head = list_insert_front(l);
        *head = 1;

        CHECK_BOOL(
            l->head != NULL,
            "Head is not null after first front insert.",
            ""
        );

        CHECK_BOOL(
            l->head == l->tail,
            "Head and tail matches after first front insert.",
            ""
        );

        CHECK_BOOL(
            *(int*)l->head->data == 1,
            "Head data is correctly updated after value assignment.",
            ""
        );

        CHECK_BOOL(
            l->count == 1,
            "Count is incremented after first front insert.",
            ""
        );

        list_free(l);
    };

    TEST_SECTION("List multiple front insert") {

        l = list_alloc(sizeof(int), MAX_NODES);

        int* first  = list_insert_front(l);
        int* second = list_insert_front(l);
        int* third  = list_insert_front(l);

        *first = 1;
        *second = 2;
        *third = 3;

        CHECK_BOOL(
            l->tail->next == NULL,
            "Tail next pointer is NULL after multiple front inserts.",
            ""
        );

        CHECK_BOOL(
            l->head == l->head->next->prev && 
            l->tail->prev == l->head->next,
            "Nodes pointers are correct after multiple front inserts.",
            ""
        );

        CHECK_BOOL(
            *(int*)l->head->data == 3 && 
            *(int*)l->head->next->data == 2 && 
            *(int*)l->head->next->next->data == 1,
            "Nodes data is correctly updated after value assignment.",
            ""
        );

        CHECK_BOOL(
            l->count == 3,
            "Count is correct after multiple front inserts.",
            ""
        );
        
        list_free(l);
    };

    TEST_SECTION("List back insert on empty list") {

        l = list_alloc(sizeof(int), MAX_NODES);

        int* tail = list_insert_back(l);
        *tail = 1;

        CHECK_BOOL(
            l->tail != NULL,
            "Tail is not null after first back insert.",
            ""
        );

        CHECK_BOOL(
            l->head == l->tail,
            "Head and tail matches after first back insert.",
            ""
        );

        CHECK_BOOL(
            *(int*)l->tail->data == 1,
            "Tail data is correctly updated after value assignment.",
            ""
        );

        CHECK_BOOL(
            l->count == 1,
            "Count is incremented after first back insert.",
            ""
        );

        list_free(l);
    };

    TEST_SECTION("List multiple back insert") {

        l = list_alloc(sizeof(int), MAX_NODES);

        int* first  = list_insert_back(l);
        int* second = list_insert_back(l);
        int* third  = list_insert_back(l);

        *first = 1;
        *second = 2;
        *third = 3;

        CHECK_BOOL(
            l->tail->next == NULL,
            "Tail next pointer is NULL after multiple back inserts.",
            ""
        );

        CHECK_BOOL(
            l->head == l->head->next->prev && 
            l->tail->prev == l->head->next,
            "Nodes pointers are correct after multiple back inserts.",
            ""
        );

        CHECK_BOOL(
            *(int*)l->head->data == 1 && 
            *(int*)l->head->next->data == 2 && 
            *(int*)l->head->next->next->data == 3,
            "Nodes data is correctly updated after value assignment.",
            ""
        );

        CHECK_BOOL(
            l->count == 3,
            "Count is correct after multiple back inserts.",
            ""
        );
        
        list_free(l);
    };


    TEST_SECTION("List insert at arbitrary node") {

        l = list_alloc(sizeof(int), MAX_NODES);

        int* first  = list_insert_back(l);
        int* second = list_insert_back(l);
        int* third  = list_insert_back(l);
        int* fourth = list_insert_back(l);
        int* fifth  = list_insert_back(l);
        int* sixth  = list_insert_back(l);

        *first  = 1;
        *second = 2;
        *third  = 3;
        *fourth = 4;
        *fifth  = 5;
        *sixth  = 6;

        // Insert at index 1 — walks from tail since count=6, count/2=3, 1 < 3 so head walk
        int* ins1 = list_insert_at(l, 1);
        *ins1 = 10;

        // Insert at index 5 — walks from tail since count=7, count/2=3, 5 >= 3
        int* ins2 = list_insert_at(l, 5);
        *ins2 = 20;

        // Expected: 1, 10, 2, 3, 4, 20, 5, 6
        node_t* it = l->head;
        int expected[] = {1, 10, 2, 3, 4, 20, 5, 6};
        int ok = 1;
        for (int i = 0; i < 8; i++, it = it->next) {
            if (*(int*)it->data != expected[i]) { ok = 0; break; }
        }

        CHECK_BOOL(ok, "Nodes data is correct after insert_at from both ends.", "");

        CHECK_BOOL(l->count == 8, "Count is correct after insert_at.", "");

        list_free(l);
    };


    TEST_SECTION("List front delete") {

        l = list_alloc(sizeof(int), MAX_NODES);

        int* head = list_insert_front(l);
        *head = 1;

        int removed = 0;
        list_delete_front(l, &removed);

        CHECK_BOOL(
            removed == 1,
            "Removed value matches value.",
            ""
        );

        CHECK_BOOL(
            l->count == 0,
            "Count is updated after delete.",
            ""
        );

        list_free(l);
    };

    TEST_SECTION("List back delete") {

        l = list_alloc(sizeof(int), MAX_NODES);

        int* head = list_insert_front(l);
        int* tail = list_insert_back(l);
        *head = 1;
        *tail = 2;

        int removed = 0;
        list_delete_back(l, &removed);

        CHECK_BOOL(
            removed == 2,
            "Removed value matches value.",
            ""
        );

        CHECK_BOOL(
            l->count == 1,
            "Count is updated after delete.",
            ""
        );

        list_free(l);
    };

    TEST_SECTION("List delete at arbitrary node") {

        l = list_alloc(sizeof(int), MAX_NODES);

        int* first  = list_insert_back(l);
        int* second = list_insert_back(l);
        int* third  = list_insert_back(l);

        *first = 1;
        *second = 2;
        *third = 3;

        int removed = 0;
        list_delete_at(l, 1, &removed);

        CHECK_BOOL(
            l->head->next == l->tail, 
            "Nodes pointers are correct after deletion.",
            ""
        );

        int at_first  = *(int*)l->head->data;
        int at_last = *(int*)l->tail->data;

        CHECK_BOOL(
            at_first == 1 && 
            at_last  == 3,
            "Nodes data is correctly updated after deletion.",
            ""
        );

        CHECK_BOOL(
            l->count == 2,
            "Count is correct after multiple inserts.",
            ""
        );
        
        list_free(l);
    };

    TEST_SUMMARY();

    exit(EXIT_SUCCESS);
}
