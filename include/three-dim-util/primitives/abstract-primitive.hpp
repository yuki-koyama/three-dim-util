#ifndef abstract_primitive_hpp
#define abstract_primitive_hpp

#include <three-dim-util/opengl2/gl.hpp>
#include <iostream>
#include <Eigen/Core>

namespace threedimutil
{
    class AbstractPrimitive
    {
    public:
        // static void Initialize();
        // static void Draw();
        // static AbstractPrimitive& GetInstance();
        
    protected:
        void InitializeInternal()
        {
            if (ready_)
            {
                std::cerr << "Warning: Initialize() is called but it is already initialized. This call is ignored." << std::endl;
                return;
            }
            
            CreateVertexData();
            
            glGenBuffers(1, &vertex_vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
            
            glGenBuffers(1, &normal_vbo_);
            glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_);
            glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * normals_.size(), normals_.data(), GL_STATIC_DRAW);
            
            ready_ = true;
        }
        
        void DrawInternal()
        {
            if (!ready_) { InitializeInternal(); }
            
            glEnableClientState(GL_VERTEX_ARRAY);
            glEnableClientState(GL_NORMAL_ARRAY);
            
            glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
            glVertexPointer(3, GL_DOUBLE, 0, NULL);
            
            glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_);
            glNormalPointer(GL_DOUBLE, 0, NULL);
            
            glDrawArrays(GL_TRIANGLES, 0, vertices_.cols());
            
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            glDisableClientState(GL_VERTEX_ARRAY);
            glDisableClientState(GL_NORMAL_ARRAY);
        }
        
        AbstractPrimitive() = default;
        ~AbstractPrimitive()
        {
            if (ready_)
            {
                glDeleteBuffers(1, &vertex_vbo_);
                glDeleteBuffers(1, &normal_vbo_);
            }
        }
        AbstractPrimitive(const AbstractPrimitive&)            = delete;
        AbstractPrimitive& operator=(const AbstractPrimitive&) = delete;
        AbstractPrimitive(AbstractPrimitive&&)                 = delete;
        AbstractPrimitive& operator=(AbstractPrimitive&&)      = delete;
        
        Eigen::MatrixXd vertices_;
        Eigen::MatrixXd normals_;
        
    private:
        virtual void CreateVertexData() = 0;
        
        bool ready_ = false;
        
        GLuint vertex_vbo_;
        GLuint normal_vbo_;
    };
}

#endif /* abstract_primitive_hpp */
