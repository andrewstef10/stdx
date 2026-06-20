#ifndef CONTAINER_H
#define CONTAINER_H

namespace stdx {

    /// @brief A container provides a CRTP base class for all data structures
    /// @tparam Derived The derived class implementing this base class
    template<typename Derived>
    class container
    {
    public:

        /// @brief Returns the number of elements in the container.
        /// @details Complexity: Constant
        /// @return The number of elements in the container.
        std::size_t size() const noexcept { return m_size; }

        /// @brief Checks if the container has no elements, i.e., whether begin() == end().
        /// @details Complexity: Constant
        /// @return true if the container is empty, false otherwise.
        bool empty() const noexcept { return m_size == 0; }

        // TODO swap ???

    protected:
        /// @brief Current number of elements stored in the container
        std::size_t m_size{};

    private:
        friend Derived;

        // ==== container should not be constructed directly ====
        container() = default;
        ~container() = default;
        container(const container& other) {};

        container(container&& other) noexcept
        : m_size(other.m_size)
        { 
            other.m_size = 0; 
        }

        container& operator=(const container& other) = default;

        container& operator=(container&& other) noexcept
        { 
            m_size = other.m_size;
            other.m_size = 0;
            return *this;
        }
    };


}

#endif