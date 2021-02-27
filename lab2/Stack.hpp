//
// Created by Sophia on 24.02.2021.
//
#pragma once

#include <vector>

using namespace std;

template<typename T>

class Stack {
    vector<T> storage;
public:

    void push(const T &value) {
        storage.push_back(value);
    }

    bool isEmpty() {
        return storage.empty();
    }

    T pop() {
        if (isEmpty()) {
            throw std::length_error("Impossible to pop from empty stack");
        }
        T elem = storage.back();
        storage.pop_back();
        return elem;
    }

    T peek() {
        if (isEmpty()) throw std::length_error("Getting head of empty stack");
        return storage.back();
    }

    size_t size() {
        return storage.size();
    }

    void print() {
        for (auto &i : storage) {
            cout << i;
        }
    }

};

