#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <vector>
#include <list>
#include <fstream>
#include <stdexcept>
#include <algorithm>

template <typename T>
class priority_queue {
private: 
    struct HeapNode {
        int priority;
        std::list<T> elements;
        
        HeapNode(int p) : priority(p) {}
        HeapNode(int p, const T& element) : priority(p) {
            elements.push_back(element);
        }
    };
    
    std::vector<HeapNode> heap;
    size_t total_elements;
    
    size_t parent(size_t i) const { return (i - 1) / 2; }
    size_t left_child(size_t i) const { return 2 * i + 1; }
    size_t right_child(size_t i) const { return 2 * i + 2; }
    
    void heapify_up(size_t index);
    void heapify_down(size_t index);
    void build_heap();
    
public:
    priority_queue();
    
    priority_queue(const priority_queue& other);
    priority_queue& operator=(const priority_queue& other);
    ~priority_queue();
    
    size_t size() const;
    T& head();
    const T& head() const;
    void enqueue(const T& element, int priority);
    void dequeue();
    void clear();
    void swap(priority_queue& other) noexcept;
    
    void serialize(std::ofstream& out) const;
    void deserialize(std::ifstream& in);
    
    bool empty() const { return total_elements == 0; }
};

template <typename T>
priority_queue<T>::priority_queue() : total_elements(0) {}

template <typename T>
priority_queue<T>::priority_queue(const priority_queue& other) 
    : heap(other.heap), total_elements(other.total_elements) {}

template <typename T>
priority_queue<T>& priority_queue<T>::operator=(const priority_queue& other) {
    if (this != &other) {
        priority_queue temp(other);
        swap(temp);
    }
    return *this;
}

template <typename T>
priority_queue<T>::~priority_queue() {
    clear();
}

template <typename T>
size_t priority_queue<T>::size() const {
    return total_elements;
}

template <typename T>
T& priority_queue<T>::head() {
    if (empty()) {
        throw std::out_of_range("Queue is empty");
    }
    return heap[0].elements.front();
}

template <typename T>
const T& priority_queue<T>::head() const {
    if (empty()) {
        throw std::out_of_range("Queue is empty");
    }
    return heap[0].elements.front();
}

template <typename T>
void priority_queue<T>::heapify_up(size_t index) {
    while (index > 0) {
        size_t parent_idx = parent(index);
        if (heap[index].priority < heap[parent_idx].priority) {
            std::swap(heap[index], heap[parent_idx]);
            index = parent_idx;
        } else {
            break;
        }
    }
}

template <typename T>
void priority_queue<T>::heapify_down(size_t index) {
    while (true) {
        size_t left = left_child(index);
        size_t right = right_child(index);
        size_t smallest = index;
        
        if (left < heap. size() && heap[left].priority < heap[smallest].priority) {
            smallest = left;
        }
        if (right < heap.size() && heap[right].priority < heap[smallest].priority) {
            smallest = right;
        }
        
        if (smallest != index) {
            std::swap(heap[index], heap[smallest]);
            index = smallest;
        } else {
            break;
        }
    }
}

template <typename T>
void priority_queue<T>::enqueue(const T& element, int priority) {
    for (size_t i = 0; i < heap.size(); ++i) {
        if (heap[i].priority == priority) {
            heap[i].elements. push_back(element);
            ++total_elements;
            return;
        }
    }
    
    heap.push_back(HeapNode(priority, element));
    ++total_elements;
    heapify_up(heap.size() - 1);
}

template <typename T>
void priority_queue<T>::dequeue() {
    if (empty()) {
        throw std::out_of_range("Queue is empty");
    }
    
    heap[0].elements.pop_front();
    --total_elements;
    
    if (heap[0]. elements.empty()) {
        if (heap. size() > 1) {
            heap[0] = heap.back();
            heap.pop_back();
            heapify_down(0);
        } else {
            heap.pop_back();
        }
    }
}

template <typename T>
void priority_queue<T>::clear() {
    heap.clear();
    total_elements = 0;
}

template <typename T>
void priority_queue<T>:: swap(priority_queue& other) noexcept {
    heap.swap(other.heap);
    std::swap(total_elements, other.total_elements);
}

template <typename T>
void priority_queue<T>::serialize(std::ofstream& out) const {
    if (!out.good()) {
        throw std::runtime_error("Output stream is not valid");
    }
    
    out << total_elements << "\n";
    
    out << heap.size() << "\n";
    
    for (const auto& node :  heap) {
        out << node.priority << " ";
        out << node.elements.size() << " ";
        for (const auto& element : node.elements) {
            out << element << " ";
        }
        out << "\n";
    }
    
    if (!out.good()) {
        throw std::runtime_error("Error writing to stream");
    }
}

template <typename T>
void priority_queue<T>::deserialize(std::ifstream& in) {
    if (!in.good()) {
        throw std::runtime_error("Input stream is not valid");
    }
    
    priority_queue temp;
    
    in >> temp.total_elements;
    if (!in.good()) {
        throw std::runtime_error("Error reading total elements count");
    }
    
    size_t heap_size;
    in >> heap_size;
    if (!in.good()) {
        throw std::runtime_error("Error reading heap size");
    }
    
    temp.heap. reserve(heap_size);
    
    for (size_t i = 0; i < heap_size; ++i) {
        int priority;
        in >> priority;
        if (! in.good()) {
            throw std::runtime_error("Error reading priority");
        }
        
        temp.heap.push_back(HeapNode(priority));
        
        size_t list_size;
        in >> list_size;
        if (!in. good()) {
            throw std:: runtime_error("Error reading list size");
        }
        
        for (size_t j = 0; j < list_size; ++j) {
            T element;
            in >> element;
            if (!in.good()) {
                throw std::runtime_error("Error reading element");
            }
            temp.heap[i]. elements.push_back(element);
        }
    }
    
    swap(temp);
}
#endif