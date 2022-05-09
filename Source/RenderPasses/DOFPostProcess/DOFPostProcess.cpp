/***************************************************************************
 # Copyright (c) 2015-21, NVIDIA CORPORATION. All rights reserved.
 #
 # Redistribution and use in source and binary forms, with or without
 # modification, are permitted provided that the following conditions
 # are met:
 #  * Redistributions of source code must retain the above copyright
 #    notice, this list of conditions and the following disclaimer.
 #  * Redistributions in binary form must reproduce the above copyright
 #    notice, this list of conditions and the following disclaimer in the
 #    documentation and/or other materials provided with the distribution.
 #  * Neither the name of NVIDIA CORPORATION nor the names of its
 #    contributors may be used to endorse or promote products derived
 #    from this software without specific prior written permission.
 #
 # THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS "AS IS" AND ANY
 # EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 # IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 # PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 # CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 # EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 # PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 # PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 # OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 # (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 # OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **************************************************************************/
#include "DOFPostProcess.h"

const RenderPass::Info DOFPostProcess::kInfo { "DOFPostProcess", "Insert pass description here." };

// Don't remove this. it's required for hot-reload to function properly
extern "C" FALCOR_API_EXPORT const char* getProjDir()
{
    return PROJECT_DIR;
}

extern "C" FALCOR_API_EXPORT void getPasses(Falcor::RenderPassLibrary& lib)
{
    lib.registerPass(DOFPostProcess::kInfo, DOFPostProcess::create);
}


namespace
{
    //const std::string kProgramFile = "RenderPasses/DepthPass/DepthPass.3d.slang";

    const std::string kTilingShader =       "RenderPasses/DOFPostProcess/Tiling.3d.slang";
    //const std::string kDilateShader =       "RenderPasses/DOFPostProcess/dilate.ps.hlsl";
    //const std::string kDownPresortShader =  "RenderPasses/DOFPostProcess/downpresort.ps.hlsl";
    //const std::string kMainPassShader =     "RenderPasses/DOFPostProcess/mainpass.ps.hlsl";
    //const std::string kPPMergeShader =      "RenderPasses/DOFPostProcess/ppmerge.ps.hlsl";
    //const std::string kMotionVecShader =    "RenderPasses/DOFPostProcess/motionVec.ps.hlsl";

    const std::string kDepth = "depth";
    const std::string kTiledDepth = "tiledDepth";
}


DOFPostProcess::SharedPtr DOFPostProcess::create(RenderContext* pRenderContext, const Dictionary& dict)
{
    SharedPtr pPass = SharedPtr(new DOFPostProcess());
    return pPass;
}


Dictionary DOFPostProcess::getScriptingDictionary()
{
    return Dictionary();
}


DOFPostProcess::DOFPostProcess()
    : RenderPass(kInfo)
{
    //Program::Desc desc;
    //desc.addShaderLibrary(kTilingShader)./*vsEntry("vsMain").*/psEntry("psMain");
    //mpTilingProgram = GraphicsProgram::create(desc);
    mpPass = FullScreenPass::create(kTilingShader);
    //mpTilingProgram = GraphicsProgram::createFromFile("kTilingShader", "vsMain", "psMain");
    //mpVars = GraphicsVars::create(mpTilingProgram->getReflector());
    //mpState = GraphicsState::create();
    //mpState->setProgram(mpTilingProgram);
    mpFbo = Fbo::create();

    //parseDictionary(dict);
}


RenderPassReflection DOFPostProcess::reflect(const CompileData& compileData)
{
    // Define the required resources here
    RenderPassReflection reflector;

    int32_t width = 1920;
    int32_t height = 1080;

    // We could use instead to get size of swap chqin: compileData.defaultTexDims

    reflector.addOutput(kTiledDepth, "Tiled Depth-buffer").bindFlags(Resource::BindFlags::DepthStencil).format(mTiledDepthFormat).texture2D(width / 20, height / 20);
    auto& depthField = reflector.addInputOutput(kDepth, "Depth-buffer. Should be pre-initialized or cleared before calling the pass").bindFlags(Resource::BindFlags::DepthStencil);
    //reflector.addOutput("dst");
    //reflector.addInput("src");
    return reflector;
}


void DOFPostProcess::execute(RenderContext* pRenderContext, const RenderData& renderData)
{
    // renderData holds the requested resources
    // auto& pTexture = renderData["src"]->asTexture();

    mpFbo->attachColorTarget(renderData[kTiledDepth]->asTexture(), 0);
    mpFbo->attachDepthStencilTarget(renderData[kDepth]->asTexture());

    pRenderContext->clearRtv(mpFbo->getRenderTargetView(0).get(), float4(0));

    //mpVars["gGBuffer"] = GBuffer;
    //mpVars["gBlurredGBuffer"] = blurredGBuffer;
    auto pCB = mpPass["cameraParametersCB"];
    pCB["cameraParametersCB"]["gFocalLength"] = mpScene->getCamera()->getFocalLength();
    pCB["cameraParametersCB"]["gDistanceToFocalPlane"] = mpScene->getCamera()->getFocalDistance();
    pCB["cameraParametersCB"]["gAperture"] = mpScene->getCamera()->getApertureRadius();
    pCB["cameraParametersCB"]["gSensorWidth"] = mpScene->getCamera()->getFrameWidth();
    pCB["cameraParametersCB"]["gTextureWidth"] = (float)renderData[kTiledDepth]->asTexture()->getWidth();

    mpPass->execute(pRenderContext, mpFbo);
}

// Need to set the current scene to retrieve infos
void DOFPostProcess::setScene(RenderContext* pRenderContext, const Scene::SharedPtr& pScene)
{
    mpScene = pScene;
}

void DOFPostProcess::renderUI(Gui::Widgets& widget)
{
}
