//
//  EulerStepSolver.h
//  ClothingSimulation
//
//  Created by Jeehoon Hyun on 30/03/2019.
//  Copyright © 2019 Jeehoon Hyun. All rights reserved.
//

#ifndef EulerStepSolver_h
#define EulerStepSolver_h
#include "ParticleSystem.h"


#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class EulerStepSolver{
private:
    unsigned int VAO, VBO, EBO;
    ParticleSystem * particleSystem;
    void formBuffer(){
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        
        glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(VERTEX), &out_vertices[0], GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, out_indices.size()*sizeof(unsigned int), &out_indices[0], GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void *)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void *)offsetof(VERTEX, uv_coord));
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void *)offsetof(VERTEX, normal));
        
        glBindVertexArray(0);
    }
    //Modify render data(out_vertices) based on simulation data
    void modifyRenderData(){
        for(int i=0;i<particles.size();i++){
            for(int j=0;j<particles[i].vertexptr.size();j++){
                particles[i].vertexptr[j]->position = particles[i].position;
            }
        }
    }
    
    //Call this after our out_vertices is modified
    void modifyBuffer(){
        glBindVertexArray(VAO);
        glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(VERTEX), &out_vertices[0], GL_STATIC_DRAW);
        glBindVertexArray(0);
    }
    
    //Assign simulation data to render data
    void assignParticles(){
        for(int i=0;i<particles.size();i++){
            for(int j=0;j<out_vertices.size();j++){
                if(glm::distance(particles[i].position, out_vertices[j].position)==0.0){
                    particles[i].vertexptr.push_back(&out_vertices[j]);
                }
            }
        }
    }
    void scaleVector(std::vector<ParticleDimensionHolder>& temp1, float deltaT){
        for(int i=0;i<temp1.size();i++){
            //now this is delta x
            temp1[i].x_v = temp1[i].x_v * deltaT;
            //now this is delta v
            temp1[i].v_a = temp1[i].v_a * deltaT;
        }
    }
    //add temp1 and temp2 and store it in temp2
    void addVectors(std::vector<ParticleDimensionHolder>& temp1, std::vector<ParticleDimensionHolder>& temp2){
        for(int i=0;i<temp1.size();i++){
            temp2[i].x_v = temp2[i].x_v + temp1[i].x_v;
            temp2[i].v_a = temp2[i].v_a + temp1[i].v_a;
        }
    }
public:
    std::vector<Particle> particles;
    std::vector<VERTEX> out_vertices;
    std::vector<unsigned int> out_indices;
    std::vector<unsigned int> original_vertex_indices;
    EulerStepSolver(std::vector<Particle> particles, std::vector<VERTEX> out_vertices, std::vector<unsigned int> out_indices, std::vector<unsigned int> original_vertex_indices){
        this->particles = particles;
        this->out_vertices = out_vertices;
        this->out_indices = out_indices;
        this->original_vertex_indices = original_vertex_indices;
        assignParticles();
        formBuffer();
        particleSystem = new ParticleSystem(&(this->particles), &(this->original_vertex_indices));
    }
    
    void EulerStep(float deltaT){
        particleSystem->clearForce();
        particleSystem->computeForces();
        //std::cout << "particleSystem force of random particle" << glm::to_string((*particleSystem->particles)[90].force) << std::endl;
        std::vector<ParticleDimensionHolder> temp1 = particleSystem->getDerivative();
        scaleVector(temp1, deltaT);
        std::vector<ParticleDimensionHolder> temp2 = particleSystem->getState();
        addVectors(temp1, temp2);
        particleSystem->setState(temp2);
        particleSystem->applyConstraints();
        modifyRenderData();
    }
    
    void draw(Shader ourShader){
        ourShader.setVec3("lightDirection", glm::vec3(0.2f, 1.0f, 0.3f));
        //Explodes from 0.07. If I add the 2 point constraints, it becomes even lower than that.
        EulerStep(0.001);
        modifyBuffer();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, out_indices.size(), GL_UNSIGNED_INT, 0);
    }
};

#endif /* EulerStepSolver_h */

//TODO in the future!
//Add more springs via 'Connection'
//Add more constraints. 2 is not enough.
