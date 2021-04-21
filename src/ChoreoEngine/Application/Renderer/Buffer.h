#pragma once

#include "cepch.h"
#include <cstdint>
#include <initializer_list>
#include <string>
#include <vector>

namespace ChoreoEngine {
    enum class ShaderDataType {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Mat3,
        Mat4,
        Int, 
        Int2,
        Int3,
        Int4,
        Bool
    };

    static uint32_t ShaderDataTypeSize(ShaderDataType type){
        switch (type){
            case ShaderDataType::Float:       return 4;
            case ShaderDataType::Float2:      return 4 * 2;
            case ShaderDataType::Float3:      return 4 * 3;
            case ShaderDataType::Float4:      return 4 * 4;
            case ShaderDataType::Mat3:        return 4 * 3 * 3;
            case ShaderDataType::Mat4:        return 4 * 4 * 4;
            case ShaderDataType::Int:         return 4;
            case ShaderDataType::Int2:        return 4 * 2;
            case ShaderDataType::Int3:        return 4 * 3;
            case ShaderDataType::Int4:        return 4 * 4;
            case ShaderDataType::Bool:        return 1;
            default:
                                            return 0;
        }

        CE_CORE_ASSERT(false, "Unknown ShaderDataType!")
        return 0;
    }

    class BufferElement{
    public:
        std::string name;
        ShaderDataType type;
        uint32_t offset;
        uint32_t size;
        bool normalized;

        BufferElement() {}

        BufferElement(ShaderDataType i_type,  const std::string& i_name, bool i_normalized = false ) 
            :   name{i_name}, 
                type{i_type}, 
                offset{0},
                size{ShaderDataTypeSize( i_type )},  
                normalized{i_normalized}  {
                
            }

        uint32_t getComponentCount() const { 
            switch(type){
                case ShaderDataType::Float:  return 1;
                case ShaderDataType::Float2: return 2;
                case ShaderDataType::Float3: return 3;
                case ShaderDataType::Float4: return 4;
                case ShaderDataType::Mat3:   return 3 * 3;
                case ShaderDataType::Mat4:   return 3 * 4;
                case ShaderDataType::Int:    return 1;
                case ShaderDataType::Int2:   return 2;
                case ShaderDataType::Int3:   return 3;
                case ShaderDataType::Int4:   return 4;
                case ShaderDataType::Bool:   return 1;

                default: return 0;
            } 
            CE_CORE_ASSERT(false, "Unknown ShaderDataType!")
            return 0;
        }
    };

    class BufferLayout{

    public:
        BufferLayout() {}
        BufferLayout(const std::initializer_list<BufferElement>& elements) 
            : m_elements{elements} {
           calculateOffsetAndStride(); 
        }
        inline const std::vector<BufferElement>& getElements() const { return m_elements; }
        inline uint32_t getStride() const { return m_stride; }

        std::vector<BufferElement>::iterator begin() { return m_elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_elements.end(); }

    private:
        void calculateOffsetAndStride(){
            uint32_t offset = 0;
            m_stride = 0;
            for(auto& element : m_elements){
                element.offset = offset;
                offset += element.size;
                m_stride += element.size;
            }
        }
        std::vector<BufferElement> m_elements;
        uint32_t m_stride = 0;
    };


    class VertexBuffer{
    public:

        virtual ~VertexBuffer() {};


        virtual void bind() const = 0;
        virtual void unbind() const = 0;
        virtual void setLayout(const BufferLayout& layout) = 0; 
        virtual const BufferLayout& getLayout() const = 0;

        virtual void setData(const void* data, uint32_t size) = 0;

        static Ref<VertexBuffer> create(uint32_t size);
        static Ref<VertexBuffer> create(float* vertices, uint32_t count);

        
    };

    // Curerntly ChoreoEngine only supports 32-bit index buffers
    class IndexBuffer{
    public:
        virtual ~IndexBuffer() {};

        virtual void bind() const = 0;
        virtual void unbind() const = 0;

        virtual uint32_t getCount() const = 0;

        static Ref<IndexBuffer> create(uint32_t* indices, uint32_t count);
    };

}
