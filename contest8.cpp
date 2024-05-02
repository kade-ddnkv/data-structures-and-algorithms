#include <iostream>

template <class ValueType>
class RBTree {
public:
    RBTree() {
    }

    RBTree(const RBTree &other) {
        this->root_ = other.root_ == nullptr ? nullptr : new Node(*other.root_);
        this->nodes_count_ = other.nodes_count_;
    }

    RBTree &operator=(const RBTree &other) {
        if (this == &other) {
            return *this;
        }
        this->root_ = other.root_ == nullptr ? nullptr : new Node(*other.root_);
        this->nodes_count_ = other.nodes_count_;
        return *this;
    }

    ~RBTree() {
        delete root_;
        delete array_of_in_order_values_;
    }

    size_t size() const {
        return nodes_count_;
    }

    bool empty() const {
        return nodes_count_ == 0;
    }

    void insert(const ValueType &value) {
        root_ = insertRecursive(root_, nullptr, value);
        ++nodes_count_;
        insertCase1(last_inserted_node_);
    }

    void erase(const ValueType &value) {
        Node *node_to_delete = findNodeRecursive(root_, value);
        if (node_to_delete != nullptr) {
            Node *in_order_successor = findLowerBoundUnequal(root_, value);
            // Заменяется значение в дереве.
            if (in_order_successor == nullptr) {
                eraseCase1(node_to_delete);
            } else {
                node_to_delete->value = in_order_successor->value;
                eraseCase1(in_order_successor);
            }
            --nodes_count_;
        }
    }

    ValueType *lower_bound(const ValueType &value) const {  // NOLINT
        Node *equal_node = findNodeRecursive(root_, value);
        if (equal_node != nullptr) {
            return &(equal_node->value);
        } else {
            Node *lower_bound = findLowerBoundUnequal(root_, value);
            if (lower_bound != nullptr) {
                return &(lower_bound->value);
            } else {
                return nullptr;
            }
        }
    }

    ValueType *find(const ValueType &value) const {
        Node *node = findNodeRecursive(root_, value);
        if (node != nullptr) {
            return &(node->value);
        } else {
            return nullptr;
        }
    }

    ValueType *traversal() const {
        ValueType *array = new ValueType[nodes_count_];
        int *starting_index = new int(0);
        createArrayWhileInOrderTraversal(root_, array, starting_index);
        delete starting_index;
        return array;
    }

private:
    enum Color { RED, BLACK };
    struct Node {
        ValueType value;
        Color color;
        Node *parent = nullptr;
        Node *left = nullptr;
        Node *right = nullptr;

    public:
        explicit Node(ValueType value, Node *parent, Color color) {
            this->value = value;
            this->parent = parent;
            this->color = color;
        }

        Node(const Node &other) {
            value = other.value;
            color = other.color;
            if (other.left != nullptr) {
                left = new Node(*other.left);
                left->parent = this;
            }
            if (other.right != nullptr) {
                right = new Node(*other.right);
                right->parent = this;
            }
        }

        ~Node() {
            delete left;
            delete right;
        }
    };

    Node *root_ = nullptr;
    Node *last_inserted_node_ = nullptr;
    size_t nodes_count_ = 0;
    ValueType *array_of_in_order_values_ = nullptr;

    Node *insertRecursive(Node *root, Node *parent, const ValueType &value) {
        if (root == nullptr) {
            last_inserted_node_ = new Node(value, parent, RED);
            return last_inserted_node_;
        } else if (value < root->value) {
            root->left = insertRecursive(root->left, root, value);
        } else if (value > root->value) {
            root->right = insertRecursive(root->right, root, value);
        }
        return root;
    }

    Node *grandparent(Node *n) {
        if ((n != nullptr) && (n->parent != nullptr)) {
            return n->parent->parent;
        }
        return nullptr;
    }

    Node *uncle(Node *n) {
        Node *g = grandparent(n);
        if (g == nullptr) {
            return nullptr;
        }
        if (n->parent == g->left) {
            return g->right;
        } else {
            return g->left;
        }
    }

    void rotateLeft(Node *n) {
        Node *pivot = n->right;

        pivot->parent = n->parent; /* при этом, возможно, pivot становится корнем дерева */
        if (n->parent != nullptr) {
            if (n->parent->left == n) {
                n->parent->left = pivot;
            } else {
                n->parent->right = pivot;
            }
        } else {
            // Если n был корнем дерева.
            root_ = pivot;
        }

        n->right = pivot->left;
        if (pivot->left != nullptr) {
            pivot->left->parent = n;
        }

        n->parent = pivot;
        pivot->left = n;
    }

    void rotateRight(Node *n) {
        Node *pivot = n->left;

        pivot->parent = n->parent; /* при этом, возможно, pivot становится корнем дерева */
        if (n->parent != nullptr) {
            if (n->parent->left == n) {
                n->parent->left = pivot;
            } else {
                n->parent->right = pivot;
            }
        } else {
            // Если n был корнем дерева.
            root_ = pivot;
        }

        n->left = pivot->right;
        if (pivot->right != nullptr) {
            pivot->right->parent = n;
        }

        n->parent = pivot;
        pivot->right = n;
    }

    void insertCase1(Node *n) {
        if (n->parent == nullptr) {
            n->color = BLACK;
        } else {
            insertCase2(n);
        }
    }

    void insertCase2(Node *n) {
        if (n->parent->color == BLACK) {
            return;
        } else {
            insertCase3(n);
        }
    }

    void insertCase3(Node *n) {
        Node *u = uncle(n);
        Node *g;

        if (u != nullptr && u->color == RED) {
            n->parent->color = BLACK;
            u->color = BLACK;
            g = grandparent(n);
            g->color = RED;
            insertCase1(g);
        } else {
            insertCase4(n);
        }
    }

    void insertCase4(Node *n) {
        Node *g = grandparent(n);

        if ((n == n->parent->right) && (n->parent == g->left)) {
            rotateLeft(n->parent);
            n = n->left;
        } else if ((n == n->parent->left) && (n->parent == g->right)) {
            rotateRight(n->parent);
            n = n->right;
        }
        insertCase5(n);
    }

    void insertCase5(Node *n) {
        Node *g = grandparent(n);

        n->parent->color = BLACK;
        g->color = RED;
        if ((n == n->parent->left) && (n->parent == g->left)) {
            rotateRight(g);
        } else {
            rotateLeft(g);
        }
    }

    Node *sibling(Node *n) {
        if (n == n->parent->left) {
            return n->parent->right;
        } else {
            return n->parent->left;
        }
    }

    void replaceNode(Node *removable, Node *node_to_place) {
        if (node_to_place != nullptr) {
            node_to_place->parent = removable->parent;
        }
        if (removable == removable->parent->left) {
            removable->parent->left = node_to_place;
        } else {
            removable->parent->right = node_to_place;
        }
        removable->left = nullptr;
        removable->right = nullptr;
        delete removable;
    }

    /*
    void delete_one_child(Node *n) {
        //Условие: n имеет не более одного ненулевого потомка.
        struct node *child = is_leaf(n->right) ? n->left : n->right;

        replaceNode(n, child);
        if (n->color == BLACK) {
            if (child->color == RED) {
                child->color = BLACK;
            } else {
                deleteCase6(child);
            }
        }
        free(n);
    }
     */

    void eraseCase1(Node *n) {
        if (n->color == RED) {
            // n - элемент с двумя фиктивными листовыми потомками.
            replaceNode(n, n->left);
        } else {
            eraseCase2(n);
        }
    }

    void eraseCase2(Node *n) {
        // У удаляемого элемента слева всегда фиктивная листовая вершина.
        if (n->color == BLACK && n->right != nullptr && n->right->color == RED) {
            n->right->color = BLACK;
            replaceNode(n, n->right);
        } else {
            eraseCase3(n);
        }
    }

    void eraseCase3(Node *n) {
        // Здесь: удаляемый узел черный.
        // Оба его потомка - фиктивные вершины.
        // replaceNode(n, n->right);
        deleteCase1(n);
        replaceNode(n, nullptr);
    }

    void deleteCase1(Node *n) {
        if (n->parent != nullptr) {
            deleteCase2(n);
        }
    }

    void deleteCase2(Node *n) {
        Node *s = sibling(n);

        if (s != nullptr && s->color == RED) {
            n->parent->color = RED;
            s->color = BLACK;
            if (n == n->parent->left) {
                rotateLeft(n->parent);
            } else {
                rotateRight(n->parent);
            }
        }
        deleteCase3(n);
    }

    void deleteCase3(Node *n) {
        Node *s = sibling(n);

        if ((n->parent->color == BLACK) && (s != nullptr && s->color == BLACK) &&
            (s->left == nullptr || s->left->color == BLACK) &&
            (s->right == nullptr || s->right->color == BLACK)) {
            s->color = RED;
            deleteCase1(n->parent);
        } else {
            deleteCase4(n);
        }
    }

    void deleteCase4(Node *n) {
        Node *s = sibling(n);

        if ((n->parent->color == RED) && (s != nullptr && s->color == BLACK) &&
            (s->left == nullptr || s->left->color == BLACK) &&
            (s->right == nullptr || s->right->color == BLACK)) {
            s->color = RED;
            n->parent->color = BLACK;
        } else {
            deleteCase5(n);
        }
    }

    void deleteCase5(Node *n) {
        Node *s = sibling(n);

        if (s != nullptr && s->color == BLACK) {
            if ((n == n->parent->left) && (s->right == nullptr || s->right->color == BLACK) &&
                (s->left != nullptr && s->left->color == RED)) {
                s->color = RED;
                s->left->color = BLACK;
                rotateRight(s);
            } else if ((n == n->parent->right) && (s->left == nullptr || s->left->color == BLACK) &&
                       (s->right != nullptr && s->right->color == RED)) {
                s->color = RED;
                s->right->color = BLACK;
                rotateLeft(s);
            }
        }
        deleteCase6(n);
    }

    void deleteCase6(Node *n) {
        Node *s = sibling(n);

        if (s != nullptr && s->right != nullptr && s->left != nullptr) {
            s->color = n->parent->color;
            n->parent->color = BLACK;

            if (n == n->parent->left) {
                s->right->color = BLACK;
                rotateLeft(n->parent);
            } else {
                s->left->color = BLACK;
                rotateRight(n->parent);
            }
        }
    }

    Node *findNodeRecursive(Node *root, const ValueType &value) const {
        if (root == nullptr) {
            return nullptr;
        } else if (value < root->value) {
            return findNodeRecursive(root->left, value);
        } else if (value > root->value) {
            return findNodeRecursive(root->right, value);
        } else {
            return root;
        }
    }

    Node *findLowerBoundUnequal(Node *root, const ValueType &value) {
        if (root == nullptr || root->value < value) {
            return nullptr;
        }
        Node *current_node = root;
        Node *parent_node;
        // Движение максимально влево.
        while (current_node != nullptr && value < current_node->value) {
            parent_node = current_node;
            current_node = current_node->left;
        }
        if (current_node == nullptr) {
            return parent_node;
        }
        // В данный момент current_node - это самый первый меньший элемент,
        // встреченный при движении сверху вниз.
        // Один шаг вправо, если возможно.
        if (current_node->right == nullptr) {
            return current_node->parent;
        } else {
            current_node = current_node->right;
        }
        // Движение максимально влево.
        while (current_node->left != nullptr) {
            current_node = current_node->left;
        }
        return current_node;
    }

    void writeToArrayWhileInOrderTraversal(Node *root, ValueType *array, int *index_to_place) const {
        if (root) {
            createArrayWhileInOrderTraversal(root->left, array, index_to_place);
            array[*index_to_place] = root->value;
            ++(*index_to_place);
            createArrayWhileInOrderTraversal(root->right, array, index_to_place);
        }
    }
};
