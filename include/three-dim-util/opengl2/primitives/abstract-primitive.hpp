#ifndef abstract_primitive_hpp
#define abstract_primitive_hpp

#include <iostream>
#include <Eigen/Core>
#include <QOpenGLFunctions_2_1>

namespace threedimutil
{
    class AbstractPrimitive : protected QOpenGLFunctions_2_1
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

            initializeOpenGLFunctions();
            
            CreateVertexData();
            
            this->glGenBuffers(1, &vertex_vbo_);
            this->glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);
            
            this->glGenBuffers(1, &normal_vbo_);
            this->glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_);
            this->glBufferData(GL_ARRAY_BUFFER, sizeof(GLdouble) * normals_.size(), normals_.data(), GL_STATIC_DRAW);
            
            ready_ = true;
        }
        
        void DrawInternal()
        {
            if (!ready_) { InitializeInternal(); }
            
            this->glEnableClientState(GL_VERTEX_ARRAY);
            this->glEnableClientState(GL_NORMAL_ARRAY);
            
            this->glBindBuffer(GL_ARRAY_BUFFER, vertex_vbo_);
            this->glVertexPointer(3, GL_DOUBLE, 0, NULL);
            
            this->glBindBuffer(GL_ARRAY_BUFFER, normal_vbo_);
            this->glNormalPointer(GL_DOUBLE, 0, NULL);
            
            this->glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices_.cols()));
            
            this->glBindBuffer(GL_ARRAY_BUFFER, 0);
            
            this->glDisableClientState(GL_VERTEX_ARRAY);
            this->glDisableClientState(GL_NORMAL_ARRAY);
        }
        
        AbstractPrimitive() = default;
        ~AbstractPrimitive()
        {
            if (ready_)
            {
                this->glDeleteBuffers(1, &vertex_vbo_);
                this->glDeleteBuffers(1, &normal_vbo_);
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
