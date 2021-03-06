//
// Created by LinKun on 10/15/15.
//

#include "BasicViewer/CorrespondingPointsGL.h"


namespace MapCreator {


    CorrespondingPointsGL::CorrespondingPointsGL(QOpenGLFunctions_4_1_Core *GL,
                                                 const CorrespondingPointsPair &corresponding_points_pair) :
            PrimitiveGL(GL),
            corresponding_points_pair_(corresponding_points_pair) {

    }

    CorrespondingPointsGL::~CorrespondingPointsGL() {

    }

    void CorrespondingPointsGL::Render() {

        using namespace std;
        GLint transformation_matrix_uniform_location = GL->glGetUniformLocation(shader_program_->programId(),
                                                                                "transformation_matrix");

        GL->glUniformMatrix4fv(transformation_matrix_uniform_location, 1, GL_FALSE,
                               &transformation_matrix_[0][0]);

        GL->glUseProgram(shader_program_->programId());
        GL->glBindVertexArray(vao_id_);
        GL->glDrawArrays(GL_LINES, 0, data_.size());
        GL->glBindVertexArray(0);
    }

    void CorrespondingPointsGL::SetupData() {

        ConvertData();

        assert (!data_.empty());

        GL->glGenVertexArrays(1, &vao_id_);
        GL->glGenBuffers(1, &vbo_id_);

        GL->glBindVertexArray(vao_id_);
        GL->glBindBuffer(GL_ARRAY_BUFFER, vbo_id_);
        GL->glBufferData(GL_ARRAY_BUFFER,
                         sizeof(VertexGL) * data_.size(),
                         reinterpret_cast<float *> (data_.data()),
                         GL_STATIC_DRAW);

        GL->glEnableVertexAttribArray(0);
        GL->glEnableVertexAttribArray(1);

        GL->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexGL), nullptr);
        GL->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexGL),
                                  (void *) (sizeof(VertexGL::Position)));

        GL->glBindBuffer(GL_ARRAY_BUFFER, 0);
        GL->glBindVertexArray(0);
    }


}