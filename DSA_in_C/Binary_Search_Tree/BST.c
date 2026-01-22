#include "BST.h"

int main()
{
    test_bst_normal();

    #ifdef EXTREME_TESTING_ON
        test_bst_extreme();
    #endif
    return 0;
}

void test_bst_normal()
{
    bst_t* p_bst = NULL;
    status_t status = SUCCESS;
    //data_t data[] = {100, 50, 150, 25, 75, 125, 200, 65, 130};
    data_t data[] = {100, 50, 25, 75, 150, 200, 65, 130, 125};
    data_t nonexistent_data[] = {-200, 34, 68, 98, 0xaaab, 0xf0f0f0};
    data_t succ_data, pred_data;
    
    size_t index;

    p_bst = create_bst();

    for(index = 0; index < sizeof(data)/sizeof(data[0]); ++index)
    {
        status = insert_bst(p_bst, data[index]);
        assert(status == SUCCESS);
        printf("insert_bst() for [%d] is succesfully \n", data[index]);
    }

    preorder(p_bst);
    inorder(p_bst);
    postorder(p_bst);
    
    preorder_nrc(p_bst);
    inorder_nrc(p_bst);
    postorder_nrc(p_bst);

    for(index = 0; index < sizeof(data)/sizeof(data[0]); ++index)
    {
        status = search_bst(p_bst, data[index]);
        assert(status == TRUE);
        printf("Search for [%d] is successful\n", data[index]);
    }

    for(index = 0; index < sizeof(nonexistent_data)/sizeof(nonexistent_data[0]); ++index)
    {
        status = search_bst(p_bst, nonexistent_data[index]);
        assert(status == FALSE);
        printf("Search for [%d] is not successful\n", nonexistent_data[index]);
    }

    for (index = 0; index < sizeof(data)/sizeof(data[0]); ++index)
    {
        status = inorder_predecessor(p_bst, data[index], &pred_data);
        switch(status)
        {
            case SUCCESS:
                printf("INORDER-PREDECESSOR(%d):%d\n", data[index], pred_data);
                break;
            case BST_DATA_NOT_FOUND:
                printf("%d is not present in the given BST\n", data[index]);
                break;
            case BST_NO_INORDER_PREDECESSOR:
                printf("%d is the first node in-order, therefore, it has no predecessor\n", data[index]);
                break;
            default:
                break;
        }

        status = inorder_successor(p_bst, data[index], &succ_data);
        switch(status)
        {
            case SUCCESS:
                printf("INORDER-SUCCESSOR(%d):%d\n", data[index], succ_data);
                break;
            case BST_DATA_NOT_FOUND:
                printf("%d is not present in the given BST\n", data[index]);
                break;
            case BST_NO_INORDER_SUCCESSOR:
                printf("%d is the last node in-order, therefore, it has no successor\n", data[index]);
                break;
            default:
                break;
        }        
    }
    printf("Inorder traversal before removing all elements ");
    inorder(p_bst);
    for(size_t index = 0; index < sizeof(data)/sizeof(data[0]); ++index)
    {
        status = remove_bst(p_bst, data[index]);
        assert(status == SUCCESS);
        printf("%d is succesfully removed from tree \n", data[index]);
        inorder(p_bst);
    }
    assert(p_bst -> p_root_node == NULL);

    status = destroy_bst(&p_bst);
    assert(status == SUCCESS && p_bst == NULL);

    puts("Binary seatch tree destroyed successfully \n");
    puts("Normal testing completed succesfully\n");
}

void test_bst_extreme()
{
    bst_t* p_bst = NULL;
    data_t* p_data_array = NULL;
    size_t N = 100000;

    status_t status;
    data_t nonexistent_data[] = {-100, -200, 32768, 1000000, 123456789};

    p_data_array = (data_t*)xmalloc(N * sizeof(data_t));

    srand(time(0));

    for(size_t index = 0; index < N; ++index)
    {
        p_data_array[index] = rand();
    }

    p_bst = create_bst();

    for(size_t index = 0; index < N; ++index)
    {
        status = insert_bst(p_bst, p_data_array[index]);
        assert(status == SUCCESS);
        printf("insert_bst(): succesful for [%d]\n", p_data_array[index]);
    }

    preorder(p_bst);
    inorder(p_bst);
    postorder(p_bst);

    for(size_t index = 0; index < N; ++index)
    {
        status = search_bst(p_bst, p_data_array[index]);
        assert(status == SUCCESS);
        printf("Search for [%d] is successful\n", p_data_array[index]);
    }

    for(size_t index = 0; index < sizeof(nonexistent_data)/sizeof(nonexistent_data[0]); ++index)
    {
        status = search_bst(p_bst, nonexistent_data[index]);
        assert(status == FALSE);
        printf("Search for [%d] is not successful\n", nonexistent_data[index]);
    }

    status = destroy_bst(&p_bst);
    assert(status == SUCCESS && p_bst == NULL);

    free(p_data_array);
    p_data_array = NULL;

    puts("Binary search tree destroyed succesfully\n");
    puts("Extreme testing completed succesfully.\n");
}

// Create and destroy function
bst_t* create_bst()
{
    bst_t* new_bst = NULL;

    new_bst = (bst_t*)xmalloc(sizeof(bst_t));
    new_bst->p_root_node = NULL;
    new_bst->nr_elements = 0;

    return new_bst;
}

status_t destroy_bst(bst_t** pp_bst)
{
    bst_t* p_bst = *pp_bst;
    destroy_nodelevel(p_bst->p_root_node);
    
    free(*pp_bst);
    *pp_bst = NULL;

    return SUCCESS;
}

status_t insert_bst(bst_t* p_bst, data_t new_data)
{
    bst_node_t* new_node = NULL;
    bst_node_t* run = NULL;

    new_node = get_bst_node(new_data);

    // Case 1: Empty BST
    if(p_bst -> p_root_node == NULL)
    {
        p_bst -> p_root_node = new_node;
        p_bst -> nr_elements += 1;
        return SUCCESS;
    }

    // Case 2: Non empty BST
    run = p_bst -> p_root_node;

    while(TRUE)
    {
        if(new_data <= run -> data) // Access left subtree
        {
            if(run -> left == NULL)
            {
                run -> left = new_node;
                run -> left -> parent = run;
                break;
            }
            run = run -> left;
        }
        else // Access right sutree
        {
            if(run -> right == NULL)
            {
                run -> right = new_node;
                run -> right -> parent = run;
                break;
            }
            run = run -> right;
        }
    }
    p_bst -> nr_elements += 1;
    return(SUCCESS); 
}

// Search data in BST
status_t search_bst(bst_t* p_bst, data_t search_data)
{
    bst_node_t* run = NULL;
    run = search_nodelevel(p_bst->p_root_node, search_data);

    if (run != NULL)
        return SUCCESS;

    return FALSE;
}

status_t remove_bst(bst_t* p_bst, data_t r_data)
{
    bst_node_t* r_node = NULL;
    r_node = search_nodelevel(p_bst->p_root_node, r_data);

    if(r_node == NULL)
        return (BST_DATA_NOT_FOUND);
    // Case 1: Node to be removed has left node NULL and right subtree exists
    if(r_node -> left == NULL)
    {   printf("Case 1: z -> left == NULL ");
        if(r_node -> parent == NULL)                    // Node to be delted itself is a root node
            p_bst -> p_root_node = r_node -> right;     // Make right node a root of entire bst
        else if(r_node == r_node -> parent -> left)     // z has parentand its left child of its parent
            r_node -> parent -> left = r_node -> right; // Delegate responsibility of right child to parent's left
        else if(r_node == r_node -> parent -> right)    // z has parent and its right child of parent
            r_node -> parent -> right = r_node -> right;// Delegates responsibility of right child to parent's right 

        if(r_node -> right != NULL)                     // z-> right may or may not empty therefore 
            r_node -> right -> parent = r_node -> parent; // Make z-> parent a parent of root of right subtree of z
    }
    // Case 2: Right node is NULL and left subtree exists
    else if(r_node -> right == NULL)
    {  
        printf("Case 2: z -> right == NULL ");
        if(r_node -> parent == NULL)                
            p_bst->p_root_node = r_node -> left;
        else if(r_node == r_node -> parent -> left)
            r_node -> parent -> left = r_node -> left;
        else if(r_node == r_node -> parent -> right)
            r_node -> parent -> right = r_node -> left;

        if(r_node -> left != NULL)
            r_node -> left -> parent = r_node -> parent;        
    }
    // Case 3: Both subtree of node to be deleted are not NULL
    else
    {
        // Case 3: Both subtrr of node to be deleted are not NULL
        // Inorder successor is best candidate to replace z such that it won't violate bst
        // Inorder successor's left is always NULL but right node may not be NULL
        bst_node_t* u = inorder_successor_nodelevel(r_node);
        printf("Case 3: z -> left != NULL & z -> right != NULL ");
        if(r_node -> right != u)
        {
            printf("\nCase 3 - 1: z -> right != u ");
            /* If inorder successor u of node is right child of z then we do not delegate
            responsibility of u to its parent and we do not take hold if right subtree of
            z because u is right subtree of z
            */

            /* If inorder successor 'u' of node to be deleted z and not right child of z then
                delegate responsibility of right child of u to the parent of u if right
            */
            u-> parent-> left = u->right;   // delegate u's right to u's parent
            if(u -> right != NULL)
            {
                u-> right -> parent = u -> parent; // make u's right accept u's parent as its parent
            }
            u -> right = r_node -> right; // right subtree of z is now right subtree of u
            u -> right -> parent = u; // make root of right subtree of z accept u as its parent
        }
        /*take hold of left subtree of z*/
        u -> left = r_node -> left; // left subtree of z is left subtree os u
        u -> left -> parent = u; // make left subtree of z accept u as its parent

        /*Do setting with r_node parent*/
        if(r_node -> parent == NULL)        // z is root node
            p_bst -> p_root_node = u;     // make successor of z as root node of entire tree
        else if(r_node == r_node -> parent -> left) // z has parent and it is left node ot its parent
            r_node -> parent -> left = u;
        else if (r_node == r_node -> parent -> right) // z has parent and it is right node of its parent
            r_node -> parent -> right = u;

        u -> parent = r_node -> parent;
    }

    free(r_node);
    r_node = NULL;
    p_bst -> nr_elements--;

    return SUCCESS;
}

void preorder(bst_t* p_bst)
{
    printf("PREORDER:[START]->");
    preorder_nodelevel(p_bst->p_root_node);
    puts("[END]");
}

void inorder(bst_t* p_bst)
{
    printf("INORDER:[START]->");
    inorder_nodelevel(p_bst->p_root_node);
    puts("[END]");
}

void postorder(bst_t* p_bst)
{
    printf("POSTORDER:[START]->");
    postorder_nodelevel(p_bst->p_root_node);
    puts("[END]");
}

// Preorder predessor and successor
status_t preorder_predecessor(bst_t* p_bst, data_t existing_data, data_t* p_pred_data)
{return TRUE;}
status_t preorder_successor(bst_t* p_bst, data_t exisitng_dataq, data_t p_succ_data)
{return TRUE;}

// Inorder predecessor and successor
status_t inorder_predecessor(bst_t* p_bst, data_t existing_data, data_t* p_pred_data)
{
    bst_node_t* p_existing_node = NULL;
    bst_node_t* p_pred_node = NULL;

    p_existing_node = search_nodelevel(p_bst->p_root_node, existing_data);
    if(p_existing_node == NULL)
        return(BST_DATA_NOT_FOUND);
    
    p_pred_node = inorder_predecessor_nodelevel(p_existing_node);
    if(p_pred_node == NULL)
        return(BST_NO_INORDER_PREDECESSOR);

    *p_pred_data = p_pred_node -> data;
    return SUCCESS;
}

status_t inorder_successor(bst_t* p_bst, data_t existing_data, data_t* p_succ_data)
{
    bst_node_t* p_existing_node = NULL;
    bst_node_t* p_succ_node = NULL;

    p_existing_node = search_nodelevel(p_bst-> p_root_node, existing_data);
    if(p_existing_node == NULL)
        return (BST_DATA_NOT_FOUND);
    
    p_succ_node = inorder_successor_nodelevel(p_existing_node);
    if(p_succ_node == NULL)
        return (BST_NO_INORDER_SUCCESSOR);
    
    *p_succ_data = p_succ_node -> data;

    return SUCCESS;
}

// Postorder predessor and successor
status_t postorder_predecessor(bst_t* p_bst, data_t exisitng_data, data_t* p_pred_data){return TRUE;}
status_t postorder_successor(bst_t* p_bst, data_t existing_data, data_t* p_succ_data){return TRUE;}

status_t max_bst(bst_t* p_bst, data_t* p_max_data)
{
    bst_node_t* max_data_node = NULL;

    if(p_bst -> p_root_node == NULL)
        return (BST_EMPTY);
    
    max_data_node = max_bst_nodelevel(p_bst->p_root_node);
    *p_max_data = max_data_node -> data;

    return (SUCCESS);
}

status_t min_bst(bst_t* p_bst, data_t* p_min_data)
{   
    bst_node_t* min_data_node;

    if(p_bst -> p_root_node == NULL)
        return (BST_EMPTY);
    
    min_data_node = min_bst_nodelevel(p_bst ->p_root_node);
    *p_min_data = min_data_node -> data;
    return (SUCCESS);
}

//===== Interface function got stack of BST nod epointers =====
bstptr_stack_t* create_bstptr_stack(void){
    return create_bstptr_list();
}

status_t push_bstptr(bstptr_stack_t* p_bstptr_stack, bst_node_t* p_bst_node)
{
    return (insert_bstptr_end(p_bstptr_stack, p_bst_node));
}
status_t top_bstptr(bstptr_stack_t* p_bstptr_stack, bst_node_t** pp_bst_node)
{
    return (get_bstptr_end(p_bstptr_stack, pp_bst_node)); 
}
status_t pop_bstptr(bstptr_stack_t* p_bstptr_stack, bst_node_t** pp_bst_node)
{
    return (pop_bstptr_end(p_bstptr_stack, pp_bst_node)); 
}
status_t is_bstptr_stack_empty(bstptr_stack_t* p_bstptr_Stack)
{
    return is_bstptr_list_empty(p_bstptr_Stack);
}
status_t destroy_bstptr_stack(bstptr_stack_t** pp_bstptr_stack)
{
    return destroy_bstptr_list(pp_bstptr_stack);
}
status_t get_bstptr_top_color(bstptr_stack_t* p_bstptr_stack, color_t* p_top_color)
{
    if(is_bstptr_list_empty(p_bstptr_stack) == STACK_EMPTY)
        return STACK_EMPTY;
    *p_top_color = p_bstptr_stack -> prev -> color;
    return SUCCESS;
}
status_t set_bstptr_top_color(bstptr_stack_t* p_bstptr_stack, color_t p_new_color)
{
    if(is_bstptr_list_empty(p_bstptr_stack) == STACK_EMPTY)
       return STACK_EMPTY;
    p_bstptr_stack -> prev -> color = p_new_color;
    return SUCCESS;
}
//=============================================================

//===== Interface function got queue of BST nod epointers =====
bstptr_queue_t* create_bstptr_queue(void)
{
    return create_bstptr_list();
}

status_t insert_bstptr(bstptr_queue_t* p_bstptr_queue, bst_node_t* p_bst_node)
{
    return (insert_bstptr_end(p_bstptr_queue, p_bst_node));
}
status_t peek_bstptr(bstptr_queue_t* p_bstptr_queue, bst_node_t** pp_bst_node)
{
    return (get_bstptr_start(p_bstptr_queue, pp_bst_node));
}
status_t delete_bstptr(bstptr_queue_t* p_bstptr_queue, bst_node_t** pp_bst_node)
{
    return (pop_bstptr_start(p_bstptr_queue, pp_bst_node));
}
status_t is_bstptr_queue_empty(bstptr_queue_t* p_bstptr_queue)
{
    return is_bstptr_list_empty(p_bstptr_queue);
}
status_t destroy_bstptr_queue(bstptr_queue_t** pp_bstptr_queue)
{
    return destroy_bstptr_list(pp_bstptr_queue);
}

//=============================================================

//==Interface function got linked list of BST nod epointers ==
bstptr_list_t* create_bstptr_list(void)
{
    bstptr_list_t* p_new_list = NULL;
    p_new_list = getbstptr_node(NULL);
    p_new_list -> next = p_new_list;
    p_new_list -> prev = p_new_list;

    return (p_new_list);
}

status_t insert_bstptr_start(bstptr_list_t* p_bstptr_list, bst_node_t* p_bst_node)
{
    generic_insert(p_bstptr_list, getbstptr_node(p_bst_node),p_bstptr_list->next);
    return SUCCESS;
}
status_t insert_bstptr_end(bstptr_list_t* p_bstptr_list, bst_node_t* p_bst_node)
{
    generic_insert(p_bstptr_list->prev, getbstptr_node(p_bst_node),p_bstptr_list);
    return SUCCESS;
}

status_t get_bstptr_start(bstptr_list_t* p_bstptr_list, bst_node_t** pp_bst_node)
{
    if(p_bstptr_list -> prev == p_bstptr_list && p_bstptr_list -> next == p_bstptr_list)
        return LIST_EMPTY;
    
    *pp_bst_node = p_bstptr_list -> next -> p_bst_node;
    return (SUCCESS);
}
status_t get_bstptr_end(bstptr_list_t* p_bstptr_list, bst_node_t** pp_bst_node)
{
    if(p_bstptr_list -> prev == p_bstptr_list && p_bstptr_list -> next == p_bstptr_list)
        return LIST_EMPTY;
    
    *pp_bst_node = p_bstptr_list -> prev -> p_bst_node;
    return (SUCCESS);
}
status_t pop_bstptr_start(bstptr_list_t* p_bstptr_list, bst_node_t** pp_bst_node)
{
    if(p_bstptr_list -> prev == p_bstptr_list && p_bstptr_list -> next == p_bstptr_list)
       return LIST_EMPTY;
    
    *pp_bst_node = p_bstptr_list -> next -> p_bst_node;
    generic_delete(p_bstptr_list-> next);

    return SUCCESS;
}

status_t pop_bstptr_end(bstptr_list_t* p_bstptr_list, bst_node_t** pp_bst_node)
{
    if(p_bstptr_list -> prev == p_bstptr_list && p_bstptr_list -> next == p_bstptr_list)
       return LIST_EMPTY;
    
    *pp_bst_node = p_bstptr_list -> prev -> p_bst_node;
    generic_delete(p_bstptr_list-> prev);

    return SUCCESS;   
}

status_t is_bstptr_list_empty(bstptr_list_t* p_bstptr_list)
{
    if (p_bstptr_list -> prev == p_bstptr_list && p_bstptr_list -> next == p_bstptr_list)
        return LIST_EMPTY;
    return FALSE;
}

status_t destroy_bstptr_list(bstptr_list_t** pp_list)
{
    bstptr_list_t* p_bstptr_list = NULL; 
    bstptr_node_t* run = NULL; 
    bstptr_node_t* run_next = NULL; 

    p_bstptr_list = *pp_list; 
    for(run = p_bstptr_list->next; run != p_bstptr_list; run = run_next)
    {
        run_next = run->next; 
        free(run); 
    }

    free(p_bstptr_list); 
    *pp_list = NULL; 

    return (SUCCESS);    
}
//=============================================================

//==Auxillary function got linked list of BST nod epointers ==
void generic_insert(bstptr_node_t* p_beg, bstptr_node_t* p_mid, bstptr_node_t* p_end)
{
    p_mid -> next = p_end;
    p_mid -> prev = p_beg;
    p_beg -> next = p_mid;
    p_end -> prev = p_mid;
}

void generic_delete(bstptr_node_t* p_delete_node)
{
    p_delete_node -> prev -> next = p_delete_node -> next;
    p_delete_node -> next -> prev = p_delete_node -> prev;

    free(p_delete_node);
    p_delete_node = NULL;
}
//=============================================================

// BST traversals - non-recursive
void preorder_nrc(bst_t* p_bst)
{
    bst_node_t* run = NULL;
    bstptr_stack_t* p_bstptr_stack = NULL;
    status_t status;
    p_bstptr_stack = create_bstptr_stack();

    run = p_bst -> p_root_node;

    printf("PREORDER_NRC: [START]->");
    while(TRUE)
    {
        while(run != NULL)
        {
            printf("[%d]->", run->data);
            push_bstptr(p_bstptr_stack, run);
            run = run -> left;
        }
        status = pop_bstptr(p_bstptr_stack, &run);
        if(status == STACK_EMPTY)
            break;
    
        run = run -> right;
    }
    puts("[END]");
    status = destroy_bstptr_stack(&p_bstptr_stack);
    assert(status == SUCCESS && p_bstptr_stack == NULL);
}

void inorder_nrc(bst_t* p_bst)
{
    bst_node_t* run = NULL;
    bstptr_stack_t* p_bstptr_stack = NULL;
    status_t status;
    p_bstptr_stack = create_bstptr_stack();

    run = p_bst -> p_root_node;

    printf("INORDER_NRC: [START]->");
    while(TRUE)
    {
        while(run != NULL)
        {
            push_bstptr(p_bstptr_stack, run);
            run = run -> left;
        }
        status = pop_bstptr(p_bstptr_stack, &run);
        if(status == STACK_EMPTY)
            break;
        
        printf("[%d]->", run->data);
        run = run -> right;
    }
    puts("[END]");
    status = destroy_bstptr_stack(&p_bstptr_stack);
    assert(status == SUCCESS && p_bstptr_stack == NULL);
}

void postorder_nrc(bst_t* p_bst)
{
    bst_node_t* run = NULL;
    bstptr_stack_t* p_bstptr_stack = NULL;
    status_t status;
    color_t color;
    p_bstptr_stack = create_bstptr_stack();

    run = p_bst -> p_root_node;

    printf("POSTORDER_NRC: [START]->");
    while(TRUE)
    {
        while(run != NULL)
        {
            push_bstptr(p_bstptr_stack, run);
            set_bstptr_top_color(p_bstptr_stack, GREY);
            run = run -> left;
        }

        if(get_bstptr_top_color(p_bstptr_stack, &color)==STACK_EMPTY)
            break;

        if(color == GREY)
        {   
            set_bstptr_top_color(p_bstptr_stack, BLACK);
            top_bstptr(p_bstptr_stack, &run);
            run = run -> right;               
            continue;
        }
        else if(color == BLACK)
        {
            pop_bstptr(p_bstptr_stack, &run);
            printf("[%d]->", run->data);
            run = NULL;            
        }
  
    }
    puts("[END]");
    status = destroy_bstptr_stack(&p_bstptr_stack);
    assert(status == SUCCESS && p_bstptr_stack == NULL);
}

// Auxillary functions
void destroy_nodelevel(bst_node_t* root_node)
{
    if(root_node != NULL)
    {
        destroy_nodelevel(root_node->left);
        destroy_nodelevel(root_node->right);
        free(root_node);
        root_node = NULL;
    }
}

bst_node_t* search_nodelevel(bst_node_t* root_node, data_t search_data)
{
    bst_node_t* run = NULL;

    run = root_node;

    while(run != NULL)
    {
        if(search_data == run -> data)
            break;
        else if(search_data < run -> data)
            run = run -> left;
        else
            run = run -> right;
    }
    return (run);
}

void preorder_nodelevel(bst_node_t* root_node)
{
    if(root_node != NULL)
    {
        printf("[%d]->", root_node->data);
        preorder_nodelevel(root_node->left);
        preorder_nodelevel(root_node->right);
    }
}

void inorder_nodelevel(bst_node_t* root_node)
{
    if(root_node != NULL)
    {
        inorder_nodelevel(root_node->left);
        printf("[%d]->", root_node->data);
        inorder_nodelevel(root_node->right);
    }
}

void postorder_nodelevel(bst_node_t* root_node)
{
    if(root_node != NULL)
    {
        postorder_nodelevel(root_node->left);
        postorder_nodelevel(root_node->right);
        printf("[%d]->", root_node->data);
    }
}

bst_node_t* inorder_predecessor_nodelevel(bst_node_t* existing_node)
{
    bst_node_t* x = NULL;
    bst_node_t* y = NULL;
    bst_node_t* z = NULL;

    z = existing_node;
    if(z -> left != NULL)
        return (max_bst_nodelevel(z -> left));
    
    x = z;
    y = x -> parent;
    while(y != NULL && y -> left == x)
    {
        x = y;
        y = y -> parent;
    }
    return (y);
}

bst_node_t* inorder_successor_nodelevel(bst_node_t* existing_node)
{
    bst_node_t* x = NULL;
    bst_node_t* y = NULL;
    bst_node_t* z = NULL;

    z = existing_node;
    if(z -> right != NULL)
        return (min_bst_nodelevel(z->right));
    
    x = z;
    y = x -> parent;

    while(y != NULL && y -> right == x)
    {
        x = y;
        y = y -> parent;
    }

    return (y);
}

bst_node_t* preorder_predecessor_nodelevel(bst_node_t* existing_node)
{
    return NULL;
}

bst_node_t* preorder_successor_nodelevel(bst_node_t* existing_node)
{
    return NULL;
}

bst_node_t* postorder_predecessot_nodelevel(bst_node_t* exisitng_node)
{
    return NULL;
}

bst_node_t* postorder_successor_nodelevel(bst_node_t* existing_node)
{
    return NULL;
}

bst_node_t* max_bst_nodelevel(bst_node_t* root_node)
{
    while(root_node -> right != NULL)
    {
        root_node = root_node->right;
    }
    return root_node;
}

bst_node_t* min_bst_nodelevel(bst_node_t* root_node)
{
    while(root_node -> left != NULL)
    {
        root_node = root_node -> left;
    }
    return root_node;
}


// General purpose functions
bst_node_t* get_bst_node(data_t new_data)
{
    bst_node_t* new_node = NULL;

    new_node = (bst_node_t*)xmalloc(sizeof(bst_node_t));
    new_node -> data = new_data;
    new_node -> left = NULL;
    new_node -> right = NULL;
    new_node -> parent = NULL;

    return new_node;
}

bstptr_node_t* getbstptr_node(bst_node_t* p_bst_node)
{
    bstptr_node_t* new_node = NULL;
    new_node = (bstptr_node_t*)xmalloc(sizeof(bstptr_node_t));
    new_node -> p_bst_node = p_bst_node;
    new_node -> color = WHITE;
    new_node -> prev = NULL;
    new_node -> next = NULL;
    return new_node;
}

void* xmalloc(size_t size)
{
    void* ptr = NULL;

    ptr = malloc(size);
    if(NULL == ptr)
    {
        fprintf(stderr, "malloc():fatal:out of memory");
        exit(EXIT_FAILURE);
    }

    return ptr;
}