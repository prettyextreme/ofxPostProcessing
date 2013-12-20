/*
 *  ColorizePass.h
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
#include "ColorizePass.h"
#include "ofMain.h"

namespace itg
{
    ColorizePass::ColorizePass(const ofVec2f& aspect, float redS, float greenS, float blueS, float lum, float apply) :
    redScale(redS), greenScale(greenS), blueScale(blueS), lumForSaturation(lum), applicationFactor(apply), RenderPass(aspect, "colorize")
    {
        
        string fragShaderSrc = STRINGIFY(uniform sampler2D tex0;
                                         uniform float redScale;
                                         uniform float greenScale;
                                         uniform float blueScale;
                                         uniform float lumForSaturation;
                                         uniform float applicationFactor;
                                         
                                         void main(){
                                             vec4 color = texture2D(tex0,gl_TexCoord[0].st);
                                             vec4 targetColor = vec4(redScale, greenScale, blueScale, 1.0);
                                             
                                             float lum = 0.3 *color.g + 0.59*color.r + 0.11*color.b;
                                             
                                             vec4 shadedColor = vec4(1.0,0.0,0.0,1.0);
                                             if(lum<=lumForSaturation)
                                                 shadedColor = lum*targetColor/lumForSaturation;
                                             else{
                                                 vec4 v = vec4(1.0,1.0,1.0,1.0);
                                                 vec4 m = (v-targetColor)/(v-lumForSaturation);
                                                 vec4 b = targetColor -(m*lumForSaturation);
                                                 shadedColor = lum * m + b;
                                             }
                                             
                                             shadedColor.a = 1.0;
                                             color.a = 1.0;
                                                 
                                             gl_FragColor = mix(color,shadedColor,applicationFactor);
                                         }
                                         );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
    }
    
    
    void ColorizePass::render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        
        ofClear(0, 0, 0, 255);
        
        shader.begin();
        
        shader.setUniformTexture("tex0", readFbo, 0);
        shader.setUniform1f("redScale", redScale);
        shader.setUniform1f("greenScale", greenScale);
        shader.setUniform1f("blueScale", blueScale);
        shader.setUniform1f("lumForSaturation", lumForSaturation);
        shader.setUniform1f("applicationFactor", applicationFactor);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}