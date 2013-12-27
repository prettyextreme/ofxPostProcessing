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
#pragma once

#include "RenderPass.h"
#include "ofShader.h"

namespace itg
{
    class BuildOffsetPass : public RenderPass
    {
    public:
        static const int MAX_KERNEL_SIZE = 25;
        
        typedef shared_ptr<BuildOffsetPass> Ptr;
        
        BuildOffsetPass(const ofVec2f& aspect, float inScale = 1.0f, float outScale = 1.0f, int mode = 0);
        
        void render(ofFbo& readFbo, ofFbo& writeFbo);
        
        float getInScale() { return inScale; }
        void setInScale(float val) { inScale = val; }
        
        float getOutScale() { return outScale; }
        void setOutScale(float val) { outScale = val; }
        
        void setMode(int val) { mode = val; }
        
    private:
        
        ofShader shader;
        
        float inScale;
        float outScale;
        float mode; //0-regular, 1-mask off white, 2-mask off black
    };
}