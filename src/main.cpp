#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include "library/priority_queue.hpp"

void print_menu() {
    std::cout << "\n=== Priority Queue ===" << std::endl;
    std::cout << "1. Create queue with N random elements and serialize" << std::endl;
    std::cout << "2. Deserialize queue from file" << std::endl;
    std::cout << "3. Dequeue and display all elements" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Choose option: ";
}

void create_and_serialize() {
    int n;
    std::string filename;
    
    std::cout << "Enter number of elements (N): ";
    std::cin >> n;
    
    if (n <= 0) {
        std::cout << "Invalid number of elements!" << std::endl;
        return;
    }
    
    std::cout << "Enter output filename: ";
    std::cin >> filename;
    
    priority_queue<int> pq;
    std::mt19937 rng(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<int> value_dist(1, 1000);
    std::uniform_int_distribution<int> priority_dist(1, 50);
    
    std::cout << "\nGenerating " << n << " random elements..." << std::endl;
    for (int i = 0; i < n; ++i) {
        int value = value_dist(rng);
        int priority = priority_dist(rng);
        pq.enqueue(value, priority);
        std::cout << "Added: value=" << value << ", priority=" << priority << std::endl;
    }
    
    std::cout << "\nSerializing to file: " << filename << std::endl;
    try {
        std::ofstream out(filename, std::ios::binary);
        if (!out) {
            std::cout << "Error opening file for writing!" << std::endl;
            return;
        }
        pq.serialize(out);
        out.close();
        std::cout << "Successfully serialized " << pq.size() << " elements!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error during serialization: " << e.what() << std::endl;
    }
}

void deserialize_from_file(priority_queue<int>& pq) {
    std::string filename;
    std::cout << "Enter input filename: ";
    std::cin >> filename;
    
    try {
        std::ifstream in(filename, std::ios::binary);
        if (!in) {
            std::cout << "Error opening file for reading!" << std::endl;
            return;
        }
        pq.deserialize(in);
        in.close();
        std::cout << "Successfully deserialized " << pq.size() << " elements!" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Error during deserialization: " << e.what() << std::endl;
    }
}

void display_all_elements(priority_queue<int>& pq) {
    if (pq.empty()) {
        std::cout << "Queue is empty!" << std::endl;
        return;
    }
    
    std::cout << "\nDequeuing all elements in priority order:" << std::endl;
    std::cout << "----------------------------------------" << std::endl;
    
    // Правим копие за да не модифицираме оригиналната опашка
    priority_queue<int> temp_pq = pq;
    
    int count = 1;
    while (!temp_pq.empty()) {
        try {
            int value = temp_pq.head();
            std::cout << count << ". Value: " << value << std::endl;
            temp_pq.dequeue();
            ++count;
        } catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
            break;
        }
    }
    
    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total elements displayed: " << (count - 1) << std::endl;
}

int main() {
    priority_queue<int> pq;
    
    std::cout << "Welcome to Priority Queue Program!" << std::endl;
    
    while (true) {
        print_menu();

        std::string input;
        if (!(std::cin >> input)) {
            std::cout << "Input error. Exiting.\n";
            break;
        }

        int choice;
        try {
            choice = std::stoi(input);
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid input! Please enter a number.\n";
            continue;
        } catch (const std::out_of_range&) {
            std::cout << "Number is too large.\n";
            continue;
        }

        switch (choice) {
            case 1: 
                create_and_serialize();      
                break;
            case 2: 
                deserialize_from_file(pq);
                break;
            case 3:
                display_all_elements(pq);    
                break;
            case 4:
                std::cout << "Goodbye!\n";
                return 0;
            default:
                std::cout << "Invalid option! Please try again.\n";
        }
    }
    
    return 0;
}