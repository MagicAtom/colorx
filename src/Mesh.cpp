#include "../include/Mesh.h"

// initializes all the buffer objects/arrays
void Mesh::SetupMesh(void)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    // load data into vertex buffers
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // A great thing about structs is that their memory layout is sequential for all its items.
    // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
    // again translates to 3/2 floats which translates to a byte array.
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    // set the vertex attribute pointers
    // vertex Positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoord));
    // // vertex tangent
    // glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Tangent));
    // // vertex bitangent
    // glEnableVertexAttribArray(4);
    // glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}

void Mesh::Draw(Shader &shader)
{
    unsigned int diffuseNr  = 1;
    unsigned int specularNr = 1;
    unsigned int normalNr   = 1;
    unsigned int heightNr   = 1;
    bool hasAmbient = 0;
    bool hasDiffuse = 0;
    bool hasSpecular = 0;
    bool hasBump = 0;
   /* if (textures.size() == 0)
    {
        hasTexture = 0;
    }
    glUniform1i(glGetUniformLocation(shader.ID, "hasTexture"), hasTexture);*/
    //glUniform3f(glGetUniformLocation(shader.ID, "objectColor"), material.diffuse.x, material.diffuse.y, material.diffuse.z);
    
    //set material data to shader
    shader.setVec3("material.ambient",  material.ambient);
    shader.setVec3("material.diffuse",  material.diffuse);
    shader.setVec3("material.specular", material.specular);
    shader.setFloat("material.shininess", 32.0f);
    

    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
        string type = textures[i].type;
        if (type == "texture_diffuse")
        {
            glUniform1i(glGetUniformLocation(shader.ID, "mytex.diffuse"), i);
            hasDiffuse = 1;

        }
        else if (type == "texture_specular")
        {
            glUniform1i(glGetUniformLocation(shader.ID, "mytex.specular"), i);
            hasSpecular = 1;
        }
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }


    //set bool flags
    glUniform1i(glGetUniformLocation(shader.ID, "mytex.hasAmbient"), hasAmbient);
    glUniform1i(glGetUniformLocation(shader.ID, "mytex.hasDiffuse"), hasDiffuse);
    glUniform1i(glGetUniformLocation(shader.ID, "mytex.hasSpecular"), hasSpecular);
    glUniform1i(glGetUniformLocation(shader.ID, "mytex.hasBump"), hasBump);

    //for(unsigned int i = 0; i < textures.size(); i++)
    //{
    //    glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
    //    // retrieve texture number (the N in diffuse_textureN)
    //    string number;
    //    string name = textures[i].type;
    //    if(name == "texture_diffuse")
    //        number = std::to_string(diffuseNr++);
    //    else if(name == "texture_specular")
    //        number = std::to_string(specularNr++); // transfer unsigned int to stream
    //    else if(name == "texture_normal")
    //        number = std::to_string(normalNr++); // transfer unsigned int to stream
    //    else if(name == "texture_height")
    //        number = std::to_string(heightNr++); // transfer unsigned int to stream

    //    // now set the sampler to the correct texture unit
    //    glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
    //    // and finally bind the texture
    //    glBindTexture(GL_TEXTURE_2D, textures[i].id);
    //}
        
    // draw mesh
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);

    // always good practice to set everything back to defaults once configured.
    glActiveTexture(GL_TEXTURE0);
}


