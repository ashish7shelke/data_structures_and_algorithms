//=============Header Files =======================
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>
//=================================================

//=============Symbolic Constants =================
enum status{
    SUCCESS                     =   1,
    TRUE                        =   1,
    FALSE                       =   0,
    BST_EMPTY                   =   -1,
    BST_DATA_NOT_FOUND          =   -2,
    BST_NO_INORDER_PREDECESSOR  =   -3,
    BST_NO_INORDER_SUCCESSOR    =   -4,
    BST_NO_PREORDER_PREDECESSOR =   -5,
    BST_NO_PREORDER_SUCCESSOR   =   -6,
    BST_NO_POSTORDER_PREDECESSOR=   -7,
    BST_NO_POSTORDER_SUCCESSOR  =   -8,
    LIST_EMPTY                  =   -9,
    STACK_EMPTY                 =   -9,
    QUEUE_EMPTY                 =   -9
};

enum color
{
    WHITE                       =   0,
    GREY                        =   1,
    BLACK                       =   2
};

//============ Typedefs ===========================
typedef int                 data_t;
typedef enum status         status_t;
typedef enum color          color_t;
typedef struct bst_node     bst_node_t;
typedef struct bst          bst_t;
typedef struct bstptr_node  bstptr_node_t;
typedef bstptr_node_t       bstptr_list_t;
typedef bstptr_list_t       bstptr_stack_t;
typedef bstptr_list_t       bstptr_queue_t;
//=================================================

//========Information Structure Definition ========
struct bst_node
{
    data_t data;
    struct bst_node* left;
    struct bst_node* right;
    struct bst_node* parent;
};

struct bst
{
    bst_node_t* p_root_node;
    size_t  nr_elements;
};

// Definition of isntance node of bst node pointer
struct bstptr_node
{
    bst_node_t* p_bst_node;
    color_t color;
    struct bstptr_node* prev;
    struct bstptr_node* next;
};
//=================================================
void test_bst_normal(void); 
void test_bst_extreme(void); 

//========Interface function declarations =========
// Create and destroy data structure instance
bst_t* create_bst(void);
status_t destroy_bst(bst_t** pp_bst);

// Insert new data into BST
status_t insert_bst(bst_t* p_bst, data_t new_data);

// Search data from BST
status_t search_bst(bst_t* p_bst, data_t search_data);

// Remove data from BST
status_t remove_bst(bst_t* p_bst, data_t r_data);

// BST traversals - recursive
void preorder(bst_t* p_bst);
void inorder(bst_t* p_bst);
void postorder(bst_t* p_bst);

// Preorder predessor and successor
status_t preorder_predecessor(bst_t* p_bst, data_t existing_data, data_t* p_pred_data);
status_t preorder_successor(bst_t* p_bst, data_t exisitng_dataq, data_t p_succ_data);

// Inorder predecessor and successor
status_t inorder_predecessor(bst_t* p_bst, data_t existing_data, data_t* p_pred_data);
status_t inorder_successor(bst_t* p_bst, data_t existing_data, data_t* p_succ_data);

// Postorder predessor and successor
status_t postorder_predecessor(bst_t* p_bst, data_t exisitng_data, data_t* p_pred_data);
status_t postorder_successor(bst_t* p_bst, data_t existing_data, data_t* p_succ_data);

// Minimum and maximum
status_t max_bst(bst_t* p_bst, data_t* p_max_data);
status_t min_bst(bst_t* p_bst, data_t* p_min_data);

// BST traversals - non-recursive
void preorder_nrc(bst_t* p_bst);
void inorder_nrc(bst_t* p_bst);
void postorder_nrc(bst_t* p_bst);
//===================================================

//==========Auxillary Function declarations =========
void destroy_nodelevel(bst_node_t* root_node);

bst_node_t* search_nodelevel(bst_node_t* root_node, data_t search_data);

void preorder_nodelevel(bst_node_t* root_node);
void inorder_nodelevel(bst_node_t* root_node);
void postorder_nodelevel(bst_node_t* root_node);

bst_node_t* inorder_predecessor_nodelevel(bst_node_t* existitng_node);
bst_node_t* inorder_successor_nodelevel(bst_node_t* existing_node);

bst_node_t* preorder_predecessor_nodelevel(bst_node_t* existing_node);
bst_node_t* preorder_successor_nodelevel(bst_node_t* existing_node);

bst_node_t* postorder_predecessot_nodelevel(bst_node_t* exisitng_node);
bst_node_t* postorder_successor_nodelevel(bst_node_t* existing_node);

bst_node_t* max_bst_nodelevel(bst_node_t* root_node);
bst_node_t* min_bst_nodelevel(bst_node_t* root_node);
//=====================================================

//===== Interface function got stack of BST nod epointers =====
bstptr_stack_t* create_bstptr_stack(void);
status_t push_bstptr(bstptr_stack_t* p_bstptr_stack, bst_node_t* p_bst_node);
status_t top_bstptr(bstptr_stack_t* p_bstptr_stack, bst_node_t** pp_bst_node);
status_t pop_bstptr(bstptr_stack_t* p_bstptr_stack, bst_node_t** pp_bst_node);
status_t is_bstptr_stack_empty(bstptr_stack_t* p_bstptr_Stack);
status_t get_bstptr_top_color(bstptr_stack_t* p_bstptr_stack, color_t* p_top_color);
status_t set_bstptr_top_color(bstptr_stack_t* p_bstptr_stack, color_t p_new_color);
status_t destroy_bstptr_stack(bstptr_stack_t** pp_bstptr_stack);
//=============================================================

//===== Interface function got queue of BST nod epointers =====
bstptr_queue_t* create_bstptr_queue(void);
status_t insert_bstptr(bstptr_queue_t* p_bstptr_queue, bst_node_t* p_bst_node);
status_t peek_bstptr(bstptr_queue_t* p_bstptr_queue, bst_node_t** pp_bst_node);
status_t delete_bstptr(bstptr_queue_t* p_bstptr_queue, bst_node_t** pp_bst_node);
status_t is_bstptr_queue_empty(bstptr_queue_t* p_bstptr_queue);
status_t destroy_bstptr_queue(bstptr_queue_t** pp_bstptr_queue);
//=============================================================

//==Interface function got linked list of BST nod epointers ==
bstptr_list_t* create_bstptr_list(void);
status_t insert_bstptr_start(bstptr_list_t* p_bstptr_list, bst_node_t* p_bst_node);
status_t insert_bstptr_end(bstptr_list_t* p_bstptr_list, bst_node_t* p_bst_node);
status_t get_bstptr_start(bstptr_list_t* p_bstptr_list, bst_node_t** pp_bst_node);
status_t get_bstptr_end(bstptr_list_t* p_bstptr_list, bst_node_t** pp_bst_node);
status_t pop_bstptr_start(bstptr_list_t* p_bstptr_list, bst_node_t** pp_bst_node);
status_t pop_bstptr_end(bstptr_list_t* p_bstptr_list, bst_node_t** pp_bst_node);
status_t is_bstptr_list_empty(bstptr_list_t* p_bstptr_list);
status_t destroy_bstptr_list(bstptr_list_t** pp_list);
//=============================================================

//==Auxillary function got linked list of BST nod epointers ==
void generic_insert(bstptr_node_t* p_beg, bstptr_node_t* p_mid, bstptr_node_t* p_end);
void generic_delete(bstptr_node_t* p_delete_node);
//=============================================================

//==========General purpose Functions =================
bst_node_t* get_bst_node(data_t new_data);
bstptr_node_t* getbstptr_node(bst_node_t* p_bst_node);
void* xmalloc(size_t size);

//============ Client side functions =================
void test_bst_normal(void);
void test_bst_extreme(void);

