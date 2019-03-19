#ifndef GL_HPP
#define GL_HPP

#include <QOpenGLFunctions_2_1>

namespace threedimutil
{
    namespace internal
    {
        class OpenglFunctions : public QOpenGLFunctions_2_1
        {
        public:
            static OpenglFunctions* getInstance()
            {
                static OpenglFunctions object;
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

    inline internal::OpenglFunctions* gl() { return internal::OpenglFunctions::getInstance(); }
}

#endif // GL_HPP
