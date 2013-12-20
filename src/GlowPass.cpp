/*
 *  GlowPass.h
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
#include "GlowPass.h"
#include "ofMain.h"

namespace itg
{
    GlowPass::GlowPass(const ofVec2f& aspect) :
    RenderPass(aspect, "glow")
    {
        
//        string fragShaderSrc = STRINGIFY(uniform sampler2D tex0;
//                                         
//                                         void main(){
//                                             //vec4 offsets = texture2D( tex1, gl_TexCoord[0].st);
//                                             
//                                             float scale = 0.9;
//                                             float minusVal = 0.8;
//                                             vec4 minus = vec4(minusVal,minusVal,minusVal,minusVal);
//                                             
//                                             vec4 zeros = vec4(0.0,0.0,0.0,0.0);
//                                             vec2 ones = vec2(2.0/1024.0,0.0);
//                                             
//                                             vec4 baseColor = texture2D(tex0,gl_TexCoord[0].st);
//                                             vec4 glowColor  = 1.0 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st - 1.0*ones)-minus);
//                                             glowColor += 1.0 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st + 1.0*ones)-minus);
//                                             glowColor += 0.9 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st - 2.0*ones)-minus);
//                                             glowColor += 0.9 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st + 2.0*ones)-minus);
//                                             glowColor += 0.8 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st - 3.0*ones)-minus);
//                                             glowColor += 0.8 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st + 3.0*ones)-minus);
//                                             glowColor += 0.7 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st - 4.0*ones)-minus);
//                                             glowColor += 0.7 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st + 4.0*ones)-minus);
//                                             glowColor += 0.6 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st - 5.0*ones)-minus);
//                                             glowColor += 0.6 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st + 5.0*ones)-minus);
//                                             glowColor += 0.5 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st - 6.0*ones)-minus);
//                                             glowColor += 0.5 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st + 6.0*ones)-minus);
//                                             glowColor += 0.4 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st - 7.0*ones)-minus);
//                                             glowColor += 0.4 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st + 7.0*ones)-minus);
//                                             glowColor += 0.3 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st - 8.0*ones)-minus);
//                                             glowColor += 0.3 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st + 8.0*ones)-minus);
//                                             glowColor += 0.2 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st - 9.0*ones)-minus);
//                                             glowColor += 0.2 * scale * max(zeros,texture2D(tex0,gl_TexCoord[0].st + 9.0*ones)-minus);
//                                        /*     baseColor += max(zeros, 1.0f *scale * (texture2D(tex0,gl_TexCoord[0].st + 1)-minus));
//                                             baseColor += max(zeros, 0.5f *scale * (texture2D(tex0,gl_TexCoord[0].st - 2)-minus));
//                                             baseColor += max(zeros, 0.5f *scale * (texture2D(tex0,gl_TexCoord[0].st + 2)-minus));
//                                             baseColor += max(zeros, 0.2f *scale * (texture2D(tex0,gl_TexCoord[0].st - 3)-minus));
//                                             baseColor += max(zeros, 0.2f *scale * (texture2D(tex0,gl_TexCoord[0].st + 3)-minus));
//                                          */
//                                             glowColor.a = 1.0;
//                                             gl_FragColor = glowColor + vec4(baseColor.r , baseColor.g, baseColor.b, 1.0);
//                                         }
//                                         );
        

        string fragShaderSrc = STRINGIFY(uniform sampler2D tex0;
                                   uniform vec2 mouse;
                                   uniform vec2 resolution;

                                   vec3 highlights(vec3 pixel, float thres){
                                       float val = (pixel.x + pixel.y + pixel.z) / 3.0;
                                       return pixel * smoothstep(thres - 0.1, thres + 0.1, val);
                                   }

                                   vec3 sample(vec2 tc){
                                       return pow( texture2D(tex0, tc).xyz, vec3(2.2, 2.2, 2.2));
                                   }

                                   vec3 hsample(vec2 tc){
                                       return highlights(sample(tc), 0.6);
                                   }

                                   vec3 blur(vec2 tc, float offs){
                                       vec4 xoffs = offs * vec4(-2.0, -1.0, 1.0, 2.0)/2048.0;// / iResolution.x;
                                       vec4 yoffs = offs * vec4(-2.0, -1.0, 1.0, 2.0)/1024.0;// / iResolution.y;

                                       vec3 color = vec3(0.0, 0.0, 0.0);
                                       color += hsample(tc + vec2(xoffs.x, yoffs.x)) * 0.00366;
                                       color += hsample(tc + vec2(xoffs.y, yoffs.x)) * 0.01465;
                                       color += hsample(tc + vec2(    0.0, yoffs.x)) * 0.02564;
                                       color += hsample(tc + vec2(xoffs.z, yoffs.x)) * 0.01465;
                                       color += hsample(tc + vec2(xoffs.w, yoffs.x)) * 0.00366;

                                       color += hsample(tc + vec2(xoffs.x, yoffs.y)) * 0.01465;
                                       color += hsample(tc + vec2(xoffs.y, yoffs.y)) * 0.05861;
                                       color += hsample(tc + vec2(    0.0, yoffs.y)) * 0.09524;
                                       color += hsample(tc + vec2(xoffs.z, yoffs.y)) * 0.05861;
                                       color += hsample(tc + vec2(xoffs.w, yoffs.y)) * 0.01465;

                                       color += hsample(tc + vec2(xoffs.x, 0.0)) * 0.02564;
                                       color += hsample(tc + vec2(xoffs.y, 0.0)) * 0.09524;
                                       color += hsample(tc + vec2(    0.0, 0.0)) * 0.15018;
                                       color += hsample(tc + vec2(xoffs.z, 0.0)) * 0.09524;
                                       color += hsample(tc + vec2(xoffs.w, 0.0)) * 0.02564;

                                       color += hsample(tc + vec2(xoffs.x, yoffs.z)) * 0.01465;
                                       color += hsample(tc + vec2(xoffs.y, yoffs.z)) * 0.05861;
                                       color += hsample(tc + vec2(    0.0, yoffs.z)) * 0.09524;
                                       color += hsample(tc + vec2(xoffs.z, yoffs.z)) * 0.05861;
                                       color += hsample(tc + vec2(xoffs.w, yoffs.z)) * 0.01465;

                                       color += hsample(tc + vec2(xoffs.x, yoffs.w)) * 0.00366;
                                       color += hsample(tc + vec2(xoffs.y, yoffs.w)) * 0.01465;
                                       color += hsample(tc + vec2(    0.0, yoffs.w)) * 0.02564;
                                       color += hsample(tc + vec2(xoffs.z, yoffs.w)) * 0.01465;
                                       color += hsample(tc + vec2(xoffs.w, yoffs.w)) * 0.00366;

                                       return color;
                                   }

                                   void main(void){
                                       vec2 tc = gl_FragCoord.xy/vec2(2048.0,1024.0);
                                       vec3 color = blur(tc, 2.0);
                                       color += blur(tc, 3.0);
                                       color += blur(tc, 5.0);
                                       color += blur(tc, 7.0);
                                       //color /= 4.0;

                                       //gl_FragColor = vec4(color.r , color.g, color.b, 1.0);
                                       color += sample(tc);
            
                                       float divider = smoothstep(-0.01, 0.01, tc.x);
                                       //gl_FragColor.xyz = mix(sample(tc), color, tc.x);// * (divider * divider + (1.0 - divider) * (1.0 - divider));
                                       gl_FragColor.xyz = max(texture2D(tex0, tc).rgb, color);// * (divider * divider + (1.0 - divider) * (1.0 - divider));
                                       gl_FragColor.w = 1.0;
                                   }
                                   
                                   
                                   );
        
        shader.setupShaderFromSource(GL_FRAGMENT_SHADER, fragShaderSrc);
        shader.linkProgram();
        
    }
    
    
    void GlowPass::render(ofFbo& readFbo, ofFbo& writeFbo)
    {
        ofSetColor(255,255,255);
        
        writeFbo.begin();
        
        ofClear(0, 0, 0, 255);
        
        shader.begin();
        
        shader.setUniformTexture("tex0", readFbo, 0);
        
        texturedQuad(0, 0, writeFbo.getWidth(), writeFbo.getHeight());
        
        shader.end();
        
        writeFbo.end();
    }
}