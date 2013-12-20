/*
 *  OffsetPass.h
 *
 *  Copyright (c) 2013, satcy, http://satcy.net
 *  All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Neil Mendoza nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */
#include "OffsetPass.h"
#include "ofMain.h"

namespace itg
{
    OffsetPass::OffsetPass(const ofVec2f& aspect) :
    RenderPass(aspect, "offset")
    {
        
        string fragShaderSrc = STRINGIFY(uniform sampler2D tex0;
                                         uniform sampler2D tex1;
                                         
                                         void main(){
                                             vec4 offsets = texture2D( tex1, gl_TexCoord[0].st);
                                             vec4 color = texture2D(tex0,gl_TexCoord[0].st + (offsets.st-0.5)/4.0);
                                             
                                             
                                             gl_FragColor =  vec4(color.r , color.g, color.b, 1.0);
                                         }
                                         );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
        
        ofDisableArbTex();
        offsetImage.loadImage("offset.jpg");
        ofEnableArbTex();
        
        
        offsetFbo = NULL;
        //offsetImage.resize(2048, 1024);
    }
    
    
    void OffsetPass::render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        
        if(offsetFbo == NULL){
            //ofDisableArbTex();
            offsetFbo = new ofFbo();
            
            ofFbo::Settings s;
            s.width = ofNextPow2(2048);
            s.height = ofNextPow2(1024);
            s.textureTarget = GL_TEXTURE_2D;
            
            offsetFbo->allocate(s);
            //ofEnableArbTex();
            
            offsetFbo->begin();
            ofClear(127,127,127,255);
            offsetFbo->end();

        }
        
//        
//        offsetFbo->begin();
//        ofClear(127,127,127,255);
//        ofSetColor(0,150,0);
//        ofScale(2048/1280,1024/720);
//        for(int i=0;i<20;i++)
//            ofRectRounded(i*50, 200, 40, 100, 20);
//        //ofRect(0,0,1000,1000);
//        ofSetColor(0,0,0);
//        //ofRect(400, 400, 100, 100);
//        offsetFbo->end();
        
        ofSetColor(255,255,255);
        
        writeFbo.begin();
        
        ofClear(0, 0, 0, 255);
        
        shader.begin();
        
        shader.setUniformTexture("tex0", readFbo, 0);
        shader.setUniformTexture("tex1", offsetFbo->getTextureReference(), 1);//offsetImage.getTextureReference(), 1);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        
        writeFbo.end();
    }
}