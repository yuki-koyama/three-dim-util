#ifndef GL_HPP
#define GL_HPP

#include <QOpenGLFunctions_2_1>

namespace threedimutil
{
    namespace internal
    {
        class GlFunctions : public QOpenGLFunctions_2_1
        {
        public:
            static GlFunctions* get()
            {
                static GlFunctions object;
                if (!object.is_initialized_)
                {
                    object.initializeOpenGLFunctions();
                    object.is_initialized_ = true;
                }
                return &object;
            }
        protected:
            bool is_initialized_ = false;
        };
    }
}

#endif // GL_HPP
