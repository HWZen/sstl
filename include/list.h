//
// Created by 10414 on 2022/6/13.
//

#ifndef SSTL_LIST_H
#define SSTL_LIST_H
#include "functional.h"
#include <optional>
#include <stdexcept>



namespace sstd{

template <typename Ty1, typename Ty2>
concept ValueType = std::is_same_v<std::remove_cvref_t<Ty1> , std::remove_cvref_t<Ty2>>;

template <typename T>
class list{
public:
    using value_type = T;
    using pVal = value_type*;
    using refVal = value_type&;

    struct Node{
        std::optional<value_type> data;
        Node* next{nullptr};
        Node* prev{nullptr};

        template<ValueType<T> Type>
        constexpr explicit Node(Type&& val): data{std::forward<Type>(val)}{}
        constexpr Node(): data{},next(nullptr),prev(nullptr){}

    };


    using pNode = Node*;

    constexpr list();

    constexpr list(const list& l);

    constexpr list(list&& l) noexcept;

    constexpr list(std::initializer_list<value_type> il);

    constexpr list& operator=(const list& p);

    constexpr list& operator=(list&& p) noexcept;

    constexpr ~list();

    constexpr void clear();

    constexpr void swap(list& l);

    class const_iterator {
    public:
        const_iterator() = delete;
        constexpr const_iterator(const const_iterator& it) = default;
        constexpr const_iterator(const_iterator&& it)  noexcept = default;
        ~const_iterator() = default;
        constexpr const_iterator& operator=(const const_iterator& it) = default;
        constexpr const_iterator& operator=(const_iterator&& it) noexcept = default;

        constexpr const_iterator& operator++(){
            it = it->next;
            return *this;
        }

        constexpr const_iterator operator++(int){
            const_iterator tmp = *this;
            ++*this;
            return tmp;
        }

        constexpr const_iterator& operator--(){
            it = it->prev;
            return *this;
        }


        constexpr const const_iterator operator--(int){
            const_iterator tmp = *this;
            --*this;
            return tmp;
        }

        constexpr bool operator==(const const_iterator& other) const{
            return this->it == other.it && this->listObj == other.listObj;
        }

        constexpr bool operator!=(const const_iterator& other) const{
            return !(*this == other);
        }

        constexpr const refVal operator*() const{
            return it->data.value();
        }

        constexpr const pVal operator->() const{
            return &it->data.value();
        }

        constexpr bool checkValid() const{
            return it != nullptr && it->data.has_value();
        }

        friend class list;

    private:
        pNode it;
        list* listObj;

        constexpr explicit const_iterator(list *obj , pNode p) : it(p), listObj(obj){}
        constexpr explicit const_iterator(const list*obj, pNode p) : it(p), listObj(const_cast<list*>(obj)){}
    };

    class iterator : public const_iterator{
    public:
        iterator() = delete;
        using const_iterator::const_iterator;

        constexpr refVal operator*() const{
            return this->it->data.value();
        }
        constexpr pVal operator->() const{
            return &this->it->data.value();
        }
        constexpr iterator& operator++() {
            const_iterator::operator++();
            return *this;
        }
        constexpr iterator operator++(int){
            iterator tmp = *this;
            ++*this;
            return tmp;
        }

        constexpr iterator& operator--() {
            const_iterator::operator--();
            return *this;
        }
        constexpr iterator operator--(int) {
            iterator tmp = *this;
            --*this;
            return tmp;
        }
    };




    constexpr auto begin();

    constexpr auto end();

    constexpr const_iterator cbegin() const{
        return const_iterator(this, m_head);
    }

    constexpr const_iterator cend() const{
        return const_iterator(this, m_tail);
    }


    template<ValueType<T> Type>
    constexpr void push(Type&& val, const const_iterator &it);

    template<ValueType<T> Type>
    constexpr void push_back(Type&& val) {
        push(std::forward<Type>(val), end());
    }

    template<ValueType<T> Type>
    constexpr void push_front(Type&& val) {
        push(std::forward<Type>(val), begin());
    }


    constexpr void pop(same_or_subclass<list<T>::const_iterator> auto &&it);

    constexpr refVal front(){
        return m_head->data.value();
    }

    constexpr const refVal front() const{
        return m_head->data.value();
    }

    constexpr value_type& back(){
        if(m_tail->prev == nullptr)
            throw std::bad_optional_access();
        return m_tail->prev->data.value();
    }

    constexpr const refVal back() const{
        if(m_tail->prev == nullptr)
            throw std::bad_optional_access();
        return m_tail->prev->data.value();
    }

    constexpr size_t size() const{
        return m_size;
    }

private:
    pNode m_head{nullptr};
    pNode m_tail{nullptr};
    size_t m_size{0};
};

    template<typename T>
    constexpr list<T>::list(const list &l) :list(){
        pNode p = l.m_head;
        while(p && p->data.has_value()){
            push_back(p->data.value());
            p = p->next;
        }
    }

    template<typename T>
    constexpr list<T>::list(list &&l) noexcept {
        swap(l);
    }

    template<typename T>
    constexpr list<T>::~list() {
        pNode it = m_head;
        while(it != nullptr){
            pNode tmp = it;
            it = it->next;
            delete tmp;
        }
        m_head = m_tail = nullptr;
    }

    template<typename T>
    constexpr auto list<T>::begin() {
        return iterator(this, m_head);
    }

    template<typename T>
    constexpr auto list<T>::end() {
        return iterator(this, m_tail);
    }

    template<typename T>
    template<ValueType<T> Type>
    constexpr void list<T>::push(Type &&val, const const_iterator &it) {
        if(this != it.listObj){
            // TODO: had better do compile-time check
            return;
        }
        auto newNode = new Node(std::forward<Type>(val));
        auto node = it.it;
        newNode->next = node;
        newNode->prev = node->prev;
        if(node->prev != nullptr)
            node->prev->next = newNode;
        node->prev = newNode;
        if(node == m_head)
            m_head = newNode;
        ++m_size;
    }

    template<typename T>
    constexpr list<T>::list() {
        m_head = new Node();
        m_tail = m_head;
    }

    template<typename T>
    constexpr list<T>::list(std::initializer_list<value_type> il) : list() {
        for(auto& i : il){
            push_back(i);
        }
    }

    template<typename T>
    constexpr void list<T>::pop(same_or_subclass<typename list<T>::const_iterator> auto &&it) {
        auto node = it.it;
        if(!node->data.has_value() || it.listObj != this)
            return; // it must be ended.
        if(node->next != nullptr)
            node->next->prev = node->prev;
        if(node->prev != nullptr)
            node->prev->next = node->next;

        if(node == m_head)
            m_head = node->next;
        delete node;
        node = nullptr;
        --m_size;

    }

    template<typename T>
    constexpr void list<T>::clear() {
        pNode it = m_head;
        while(it != nullptr){
            pNode tmp = it;
            it = it->next;
            delete tmp;
        }
        m_head = m_tail = new Node();
        m_size = 0;
    }

    template<typename T>
    constexpr list<T>& list<T>::operator=(const list<T> &p) {
        clear();
        auto it{p.cbegin()};
        while(it != p.cend()){
            push_back(*it);
            ++it;
        }
        return *this;
    }

    template<typename T>
    constexpr list<T> &list<T>::operator=(list<T> &&p) noexcept{
        swap(p);
        return *this;
    }

    template<typename T>
    constexpr void list<T>::swap(list<T> &l) {
        std::swap(m_head, l.m_head);
        std::swap(m_tail, l.m_tail);
        std::swap(m_size, l.m_size);
    }

    using listInt = list<int>;
    using listDouble = list<double>;
    using listChar = list<char>;
    using listBool = list<bool>;
}


#endif //SSTL_LIST_H
