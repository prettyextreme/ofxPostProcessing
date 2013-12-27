/*
 *  BuildOffsetPass.h
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
#include "BuildOffsetPass.h"
#include "ofMain.h"

namespace itg
{
    BuildOffsetPass::BuildOffsetPass(const ofVec2f& aspect, float inScale, float outScale, int mode) :
    inScale(inScale), outScale(outScale), mode(mode), RenderPass(aspect, "buildoffset")
    {
        string fragShaderSrc = STRINGIFY(uniform sampler2D tex0;
                                         uniform float inScale;
                                         uniform float outScale;
                                         uniform int mode;
                                         
                                         void main(){
                                             
                                             vec4 thisPx = texture2D(tex0,gl_TexCoord[0].st);
                                             
                                             if(mode == 1){
                                                 if(thisPx.r+thisPx.g+thisPx.b < 3.0){
                                                     gl_FragColor =  vec4(0.5, 0.5, 0.0, 1.0);
                                                     return;
                                                 }
                                             }
                                             
                                             if(mode == 2){
                                                 if(thisPx.r+thisPx.g+thisPx.b > 0.0){
                                                     gl_FragColor =  vec4(0.5, 0.5, 0.0, 1.0);
                                                     return;
                                                 }
                                             }
                                             
                                             vec4 offPxL = texture2D(tex0,gl_TexCoord[0].st + vec2(-inScale,0));
                                             vec4 offPxR = texture2D(tex0,gl_TexCoord[0].st + vec2( inScale,0));
                                             vec4 offPxT = texture2D(tex0,gl_TexCoord[0].st + vec2(0,-inScale));
                                             vec4 offPxB = texture2D(tex0,gl_TexCoord[0].st + vec2(0, inScale));
                                             
                                             vec3 diffL = thisPx.rgb - offPxL.rgb;
                                             vec3 diffR = thisPx.rgb - offPxR.rgb;
                                             vec3 diffT = thisPx.rgb - offPxT.rgb;
                                             vec3 diffB = thisPx.rgb - offPxB.rgb;
                                             
                                             float diffBrL = 0.3 *diffL.g + 0.59*diffL.r + 0.11*diffL.b;
                                             float diffBrR = 0.3 *diffR.g + 0.59*diffR.r + 0.11*diffR.b;
                                             float diffBrT = 0.3 *diffT.g + 0.59*diffT.r + 0.11*diffT.b;
                                             float diffBrB = 0.3 *diffB.g + 0.59*diffB.r + 0.11*diffB.b;
                                             
                                             float absdL = abs(diffBrL);
                                             float absdR = abs(diffBrR);
                                             float absdT = abs(diffBrT);
                                             float absdB = abs(diffBrB);
                                             
                                             float maxDiff = max(absdL,max(absdR, max(absdT,absdB)));
                                             
                                             if(maxDiff == absdL){
                                                 //gl_FragColor =  vec4(1.0, 0.0, 0.0, 1.0);
                                                 gl_FragColor =  vec4(0.5 - outScale , 0.0, 1.0, 1.0);
                                                 return;
                                             }
                                             if(maxDiff == absdR){
                                                 //gl_FragColor =  vec4(0.0, 1.0, 0.0, 1.0);
                                                 gl_FragColor =  vec4(0.5 + outScale , 0.0, 1.0, 1.0);
                                                 return;
                                             }
                                             if(maxDiff == absdT){
                                                 //gl_FragColor =  vec4(0.0, 0.0, 1.0, 1.0);
                                                 gl_FragColor =  vec4(0.0, 0.5 - outScale , 1.0, 1.0);
                                                 return;
                                             }
                                             //else
                                             
                                             //gl_FragColor =  vec4(1.0, 1.0, 0.0, 1.0);
                                             gl_FragColor =  vec4(0.0, 0.5 + outScale , 1.0, 1.0);
                                             return;
                                             
                                         }
                                         );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
    }
    
    
    void BuildOffsetPass::render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        writeFbo.begin();
        
        ofClear(0, 0, 0, 255);
        
        shader.begin();
        
        shader.setUniformTexture("tex0", readFbo, 0);
        shader.setUniform1f("inScale", inScale);
        shader.setUniform1f("outScale", outScale);
        shader.setUniform1i("mode", mode);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        writeFbo.end();
    }
}