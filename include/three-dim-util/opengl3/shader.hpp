#ifndef THREEDIMUTIL_OPENGL3_SHADER_HPP
#define THREEDIMUTIL_OPENGL3_SHADER_HPP

#include <string>

namespace threedimutil
{
    const std::string& GetDefaultVertShaderCode()
    {
        static const std::string code = R"glsl(
#version 330

        uniform mat4 view;
        uniform mat4 projection;

        layout(location = 0) in vec3 position;
        layout(location = 1) in vec3 normal;
        out vec3 v_position;
        out vec3 v_normal;

        void main()
        {
            gl_Position = projection * view * vec4(position, 1.0);

            v_position = (view * vec4(position, 1.0)).xyz;
            v_normal   = (view * vec4(normal, 0.0)).xyz;   // This assumes there is no scaling in the view matrix
        }
        )glsl";

        return code;
    }

    const std::string& GetDefaultFragShaderCode()
    {
        static const std::string code = R"glsl(
#version 330

        uniform vec3 diffuse_color;

        in vec3 v_position;
        in vec3 v_normal;
        out vec4 frag_color;

        void main()
        {
            vec3 N = normalize(v_normal);
            vec3 V = normalize(- v_position);

            vec3 light_0_dir = normalize(vec3(+1.0, +0.2, +0.5));
            vec3 light_1_dir = normalize(vec3(-1.0, -0.2, -0.2));

            vec3 light_0_col = vec3(1.0, 1.0, 0.9);
            vec3 light_1_col = vec3(0.5, 0.6, 0.7);

            // Diffuse
            float diffuse_0 = max(dot(N, light_0_dir), 0.0);
            float diffuse_1 = max(dot(N, light_1_dir), 0.0);

            vec3 diffuse_sum = diffuse_color * light_0_col * diffuse_0 + diffuse_color * light_1_col * diffuse_1;

            // Specular
            vec3 H_0 = normalize(V + light_0_dir);
            vec3 H_1 = normalize(V + light_1_dir);

            float specular_0 = pow(max(dot(H_0, N), 0.0), 256.0);
            float specular_1 = pow(max(dot(H_1, N), 0.0), 256.0);

            vec3 specular_sum = vec3(1.0) * light_0_col * specular_0 + vec3(1.0) * light_1_col * specular_1;

            // Ambient
            vec3 ambient = vec3(0.2);

            // Return
            frag_color.xyz = specular_sum + diffuse_sum + ambient;
            frag_color.w   = 1.0;
        }
        )glsl";

        return code;
    }

    const std::string& GetNormalMapFragShaderCode()
    {
        static const std::string code = R"glsl(
#version 330

        in vec3 v_position;
        in vec3 v_normal;
        out vec4 frag_color;

        void main()
        {
            frag_color.xyz = vec3(0.5 * (1.0 + v_normal[0]), 0.5 * (1.0 + v_normal[1]), v_normal[2]);
            frag_color.w   = 1.0;
        }
        )glsl";

        return code;
    }

    const std::string& GetSingleColorFragShaderCode()
    {
        static const std::string code = R"glsl(
#version 330

        uniform vec4 color;

        in vec3 v_position;
        in vec3 v_normal;
        out vec4 frag_color;

        void main()
        {
            frag_color = color;
        }
        )glsl";

        return code;
    }
}

#endif // THREEDIMUTIL_OPENGL3_SHADER_HPP
