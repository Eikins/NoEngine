#include "Graphics/Vulkan/VulkanImGui.h"

#include "imgui.h"
#include "Editor/Editor.h"

#include "Graphics/Vulkan/VulkanUtils.hpp"

namespace Graphics
{
#pragma region ImGui Shader Sources

    //-----------------------------------------------------------------------------
    // SHADERS (from official imgui_impl_vulkan.cpp
    //-----------------------------------------------------------------------------

    // glsl_shader.vert, compiled with:
    // # glslangValidator -V -x -o glsl_shader.vert.u32 glsl_shader.vert
    /*
    #version 450 core
    layout(location = 0) in vec2 aPos;
    layout(location = 1) in vec2 aUV;
    layout(location = 2) in vec4 aColor;
    layout(push_constant) uniform uPushConstant { vec2 uScale; vec2 uTranslate; } pc;

    out gl_PerVertex { vec4 gl_Position; };
    layout(location = 0) out struct { vec4 Color; vec2 UV; } Out;

    void main()
    {
        Out.Color = aColor;
        Out.UV = aUV;
        gl_Position = vec4(aPos * pc.uScale + pc.uTranslate, 0, 1);
    }
    */
    static const uint32_t __glsl_shader_vert_spv[] =
    {
        0x07230203,0x00010000,0x00080001,0x0000002e,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x000a000f,0x00000000,0x00000004,0x6e69616d,0x00000000,0x0000000b,0x0000000f,0x00000015,
        0x0000001b,0x0000001c,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
        0x00000000,0x00030005,0x00000009,0x00000000,0x00050006,0x00000009,0x00000000,0x6f6c6f43,
        0x00000072,0x00040006,0x00000009,0x00000001,0x00005655,0x00030005,0x0000000b,0x0074754f,
        0x00040005,0x0000000f,0x6c6f4361,0x0000726f,0x00030005,0x00000015,0x00565561,0x00060005,
        0x00000019,0x505f6c67,0x65567265,0x78657472,0x00000000,0x00060006,0x00000019,0x00000000,
        0x505f6c67,0x7469736f,0x006e6f69,0x00030005,0x0000001b,0x00000000,0x00040005,0x0000001c,
        0x736f5061,0x00000000,0x00060005,0x0000001e,0x73755075,0x6e6f4368,0x6e617473,0x00000074,
        0x00050006,0x0000001e,0x00000000,0x61635375,0x0000656c,0x00060006,0x0000001e,0x00000001,
        0x61725475,0x616c736e,0x00006574,0x00030005,0x00000020,0x00006370,0x00040047,0x0000000b,
        0x0000001e,0x00000000,0x00040047,0x0000000f,0x0000001e,0x00000002,0x00040047,0x00000015,
        0x0000001e,0x00000001,0x00050048,0x00000019,0x00000000,0x0000000b,0x00000000,0x00030047,
        0x00000019,0x00000002,0x00040047,0x0000001c,0x0000001e,0x00000000,0x00050048,0x0000001e,
        0x00000000,0x00000023,0x00000000,0x00050048,0x0000001e,0x00000001,0x00000023,0x00000008,
        0x00030047,0x0000001e,0x00000002,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,
        0x00030016,0x00000006,0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040017,
        0x00000008,0x00000006,0x00000002,0x0004001e,0x00000009,0x00000007,0x00000008,0x00040020,
        0x0000000a,0x00000003,0x00000009,0x0004003b,0x0000000a,0x0000000b,0x00000003,0x00040015,
        0x0000000c,0x00000020,0x00000001,0x0004002b,0x0000000c,0x0000000d,0x00000000,0x00040020,
        0x0000000e,0x00000001,0x00000007,0x0004003b,0x0000000e,0x0000000f,0x00000001,0x00040020,
        0x00000011,0x00000003,0x00000007,0x0004002b,0x0000000c,0x00000013,0x00000001,0x00040020,
        0x00000014,0x00000001,0x00000008,0x0004003b,0x00000014,0x00000015,0x00000001,0x00040020,
        0x00000017,0x00000003,0x00000008,0x0003001e,0x00000019,0x00000007,0x00040020,0x0000001a,
        0x00000003,0x00000019,0x0004003b,0x0000001a,0x0000001b,0x00000003,0x0004003b,0x00000014,
        0x0000001c,0x00000001,0x0004001e,0x0000001e,0x00000008,0x00000008,0x00040020,0x0000001f,
        0x00000009,0x0000001e,0x0004003b,0x0000001f,0x00000020,0x00000009,0x00040020,0x00000021,
        0x00000009,0x00000008,0x0004002b,0x00000006,0x00000028,0x00000000,0x0004002b,0x00000006,
        0x00000029,0x3f800000,0x00050036,0x00000002,0x00000004,0x00000000,0x00000003,0x000200f8,
        0x00000005,0x0004003d,0x00000007,0x00000010,0x0000000f,0x00050041,0x00000011,0x00000012,
        0x0000000b,0x0000000d,0x0003003e,0x00000012,0x00000010,0x0004003d,0x00000008,0x00000016,
        0x00000015,0x00050041,0x00000017,0x00000018,0x0000000b,0x00000013,0x0003003e,0x00000018,
        0x00000016,0x0004003d,0x00000008,0x0000001d,0x0000001c,0x00050041,0x00000021,0x00000022,
        0x00000020,0x0000000d,0x0004003d,0x00000008,0x00000023,0x00000022,0x00050085,0x00000008,
        0x00000024,0x0000001d,0x00000023,0x00050041,0x00000021,0x00000025,0x00000020,0x00000013,
        0x0004003d,0x00000008,0x00000026,0x00000025,0x00050081,0x00000008,0x00000027,0x00000024,
        0x00000026,0x00050051,0x00000006,0x0000002a,0x00000027,0x00000000,0x00050051,0x00000006,
        0x0000002b,0x00000027,0x00000001,0x00070050,0x00000007,0x0000002c,0x0000002a,0x0000002b,
        0x00000028,0x00000029,0x00050041,0x00000011,0x0000002d,0x0000001b,0x0000000d,0x0003003e,
        0x0000002d,0x0000002c,0x000100fd,0x00010038
    };

    // glsl_shader.frag, compiled with:
    // # glslangValidator -V -x -o glsl_shader.frag.u32 glsl_shader.frag
    /*
    #version 450 core
    layout(location = 0) out vec4 fColor;
    layout(set=0, binding=0) uniform sampler2D sTexture;
    layout(location = 0) in struct { vec4 Color; vec2 UV; } In;
    void main()
    {
        fColor = In.Color * texture(sTexture, In.UV.st);
    }
    */
    static const uint32_t __glsl_shader_frag_spv[] =
    {
        0x07230203,0x00010000,0x00080001,0x0000001e,0x00000000,0x00020011,0x00000001,0x0006000b,
        0x00000001,0x4c534c47,0x6474732e,0x3035342e,0x00000000,0x0003000e,0x00000000,0x00000001,
        0x0007000f,0x00000004,0x00000004,0x6e69616d,0x00000000,0x00000009,0x0000000d,0x00030010,
        0x00000004,0x00000007,0x00030003,0x00000002,0x000001c2,0x00040005,0x00000004,0x6e69616d,
        0x00000000,0x00040005,0x00000009,0x6c6f4366,0x0000726f,0x00030005,0x0000000b,0x00000000,
        0x00050006,0x0000000b,0x00000000,0x6f6c6f43,0x00000072,0x00040006,0x0000000b,0x00000001,
        0x00005655,0x00030005,0x0000000d,0x00006e49,0x00050005,0x00000016,0x78655473,0x65727574,
        0x00000000,0x00040047,0x00000009,0x0000001e,0x00000000,0x00040047,0x0000000d,0x0000001e,
        0x00000000,0x00040047,0x00000016,0x00000022,0x00000000,0x00040047,0x00000016,0x00000021,
        0x00000000,0x00020013,0x00000002,0x00030021,0x00000003,0x00000002,0x00030016,0x00000006,
        0x00000020,0x00040017,0x00000007,0x00000006,0x00000004,0x00040020,0x00000008,0x00000003,
        0x00000007,0x0004003b,0x00000008,0x00000009,0x00000003,0x00040017,0x0000000a,0x00000006,
        0x00000002,0x0004001e,0x0000000b,0x00000007,0x0000000a,0x00040020,0x0000000c,0x00000001,
        0x0000000b,0x0004003b,0x0000000c,0x0000000d,0x00000001,0x00040015,0x0000000e,0x00000020,
        0x00000001,0x0004002b,0x0000000e,0x0000000f,0x00000000,0x00040020,0x00000010,0x00000001,
        0x00000007,0x00090019,0x00000013,0x00000006,0x00000001,0x00000000,0x00000000,0x00000000,
        0x00000001,0x00000000,0x0003001b,0x00000014,0x00000013,0x00040020,0x00000015,0x00000000,
        0x00000014,0x0004003b,0x00000015,0x00000016,0x00000000,0x0004002b,0x0000000e,0x00000018,
        0x00000001,0x00040020,0x00000019,0x00000001,0x0000000a,0x00050036,0x00000002,0x00000004,
        0x00000000,0x00000003,0x000200f8,0x00000005,0x00050041,0x00000010,0x00000011,0x0000000d,
        0x0000000f,0x0004003d,0x00000007,0x00000012,0x00000011,0x0004003d,0x00000014,0x00000017,
        0x00000016,0x00050041,0x00000019,0x0000001a,0x0000000d,0x00000018,0x0004003d,0x0000000a,
        0x0000001b,0x0000001a,0x00050057,0x00000007,0x0000001c,0x00000017,0x0000001b,0x00050085,
        0x00000007,0x0000001d,0x00000012,0x0000001c,0x0003003e,0x00000009,0x0000001d,0x000100fd,
        0x00010038
    };
#pragma endregion

	void VulkanImGui::Init(VulkanContext* context, float width, float height)
	{
        _context = context;
		ImGuiIO& io = ImGui::GetIO();
		io.DisplaySize = ImVec2(width, height);
		io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);
	}

	void VulkanImGui::UpdateBuffers()
	{
        // Render to generate draw buffers
        ImGui::Render();

        ImDrawData* drawData = ImGui::GetDrawData();

        // Only update buffers if there are changes
        VkDeviceSize vertexBufferSize = drawData->TotalVtxCount * sizeof(ImDrawVert);
        VkDeviceSize indexBufferSize = drawData->TotalIdxCount * sizeof(ImDrawIdx);

        if (vertexBufferSize == 0 || indexBufferSize == 0)
        {
            // Nothing to draw, don't update anything
            return;
        }

        // If vertex count or index count differs, recreate the corresponding buffer
        if ((_vertexBuffer.buffer == VK_NULL_HANDLE) || drawData->TotalVtxCount != _vertexCount)
        {
            _vertexCount = drawData->TotalVtxCount;
            _vertexBuffer.Unmap();
            _vertexBuffer.Release();
            _vertexBuffer = _context->CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            _vertexBuffer.Bind();
            _vertexBuffer.Map();
        }

        if ((_indexBuffer.buffer == VK_NULL_HANDLE) || drawData->TotalIdxCount != _indexCount)
        {
            _indexCount = drawData->TotalVtxCount;
            _indexBuffer.Unmap();
            _indexBuffer.Release();
            _indexBuffer = _context->CreateBuffer(vertexBufferSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            _indexBuffer.Bind();
            _indexBuffer.Map();
        }

        ImDrawVert* vertexMemory = static_cast<ImDrawVert*>(_vertexBuffer.mappedMemory);
        ImDrawIdx* indexMemory = static_cast<ImDrawIdx*>(_indexBuffer.mappedMemory);

        // Upload the data
        for (int i = 0; i < drawData->CmdListsCount; i++) {
            const ImDrawList* cmd_list = drawData->CmdLists[i];
            memcpy(vertexMemory, cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert));
            memcpy(indexMemory, cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx));
            vertexMemory += cmd_list->VtxBuffer.Size;
            indexMemory += cmd_list->IdxBuffer.Size;
        }

        _vertexBuffer.Flush();
        _indexBuffer.Flush();
	}

    void VulkanImGui::RecordDrawCommands(VkCommandBuffer commandBuffer)
    {
        ImGuiIO& io = ImGui::GetIO();

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipelineLayout, 0, 1, &_descriptorSet, 0, nullptr);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, _pipeline);

        VkViewport viewport = VkCreate::Viewport(0, 0, io.DisplaySize.x, io.DisplaySize.y);
        vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
        
        float editorScale = Editor::GetEditorScale();
        _pushConstants.scale = Math::Vector2(2.0f / io.DisplaySize.x, 2.0f / io.DisplaySize.y);
        _pushConstants.translation = -Math::Vector2::One;

        vkCmdPushConstants(commandBuffer, _pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstants), &_pushConstants);

        ImDrawData* drawData = ImGui::GetDrawData();
        uint32_t vertexOffset = 0;
        uint32_t indexOffset = 0;

        if (drawData->CmdListsCount > 0)
        {
            // First, bind buffers
            VkDeviceSize offsets[1] = { 0 };
            vkCmdBindVertexBuffers(commandBuffer, 0, 1, &_vertexBuffer.buffer, offsets);
            vkCmdBindIndexBuffer(commandBuffer, _indexBuffer.buffer, 0, VK_INDEX_TYPE_UINT16);

            // Then draw the regions
            for (uint32_t i = 0; i < drawData->CmdListsCount; i++)
            {
                ImDrawList* commandList = drawData->CmdLists[i];

                for (int32_t j = 0; j < commandList->CmdBuffer.Size; j++)
                {
                    ImDrawCmd* command = &commandList->CmdBuffer[j];

                    auto scissor = VkCreate::Rect2D(
                        std::max(static_cast<int32_t>(command->ClipRect.x), 0),
                        std::max(static_cast<int32_t>(command->ClipRect.y), 0),
                        static_cast<uint32_t>(command->ClipRect.z - command->ClipRect.x),
                        static_cast<uint32_t>(command->ClipRect.w - command->ClipRect.y)
                    );

                    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
                    vkCmdDrawIndexed(commandBuffer, command->ElemCount, 1, indexOffset, vertexOffset, 0);
                    indexOffset += command->ElemCount;
                }
                vertexOffset += commandList->VtxBuffer.Size;
            }
        }
    }

    void VulkanImGui::NewFrame()
    {
        ImGui::NewFrame();
    }

    void VulkanImGui::CreateResources(VkRenderPass renderPass, VkQueue queue)
    {

        ImGuiIO& io = ImGui::GetIO();
        unsigned char* fontPixels;
        int fontTexWidth, fontTexHeight;
        io.Fonts->GetTexDataAsRGBA32(&fontPixels, &fontTexWidth, &fontTexHeight);
        VkDeviceSize fontDataSize = fontTexWidth * fontTexHeight * 4 * sizeof(unsigned char);

        // Create the Image:
        {
            auto imageCreateInfo = VkCreate::ImageCreateInfo();
            imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
            imageCreateInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
            imageCreateInfo.extent.width = fontTexWidth;
            imageCreateInfo.extent.height = fontTexHeight;
            imageCreateInfo.extent.depth = 1;
            imageCreateInfo.mipLevels = 1;
            imageCreateInfo.arrayLayers = 1;
            imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
            imageCreateInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
            imageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
            imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
            imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            if (vkCreateImage(_context->device, &imageCreateInfo, nullptr, &_fontImage) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't create ImGui font image !");
            }

            VkMemoryRequirements memoryRequirements;
            vkGetImageMemoryRequirements(_context->device, _fontImage, &memoryRequirements);

            auto memoryAllocateInfo = VkCreate::MemoryAllocateInfo();
            memoryAllocateInfo.allocationSize = memoryRequirements.size;
            memoryAllocateInfo.memoryTypeIndex = _context->FindMemoryType(memoryRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
            if (vkAllocateMemory(_context->device, &memoryAllocateInfo, nullptr, &_fontMemory) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't allocate ImGui font memory !");
            }
            if (vkBindImageMemory(_context->device, _fontImage, _fontMemory, 0) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't bind ImGui font image and memory !");
            }
        }

        // Create the Image View:
        {
            VkImageViewCreateInfo info = {};
            info.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
            info.image = _fontImage;
            info.viewType = VK_IMAGE_VIEW_TYPE_2D;
            info.format = VK_FORMAT_R8G8B8A8_UNORM;
            info.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            info.subresourceRange.levelCount = 1;
            info.subresourceRange.layerCount = 1;
            if (vkCreateImageView(_context->device, &info, nullptr, &_fontView) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't create ImGui font image view !");
            }
        }

        // Upload font image data
        {
            VulkanBuffer stagingBuffer = _context->CreateStagingBuffer(fontDataSize);
            stagingBuffer.Bind();
            stagingBuffer.Map();
            memcpy(stagingBuffer.mappedMemory, fontPixels, fontDataSize);
            stagingBuffer.Flush();
            stagingBuffer.Unmap();

            auto copyCommand = _context->CreateCommandBuffer();
            
            // Set Image Layout for a Transfer operation
            VkImageMemoryBarrier transferImageLayoutBarrier[1] = {};
            transferImageLayoutBarrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            transferImageLayoutBarrier[0].dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            transferImageLayoutBarrier[0].oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
            transferImageLayoutBarrier[0].newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            transferImageLayoutBarrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            transferImageLayoutBarrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            transferImageLayoutBarrier[0].image = _fontImage;
            transferImageLayoutBarrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            transferImageLayoutBarrier[0].subresourceRange.levelCount = 1;
            transferImageLayoutBarrier[0].subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(copyCommand, VK_PIPELINE_STAGE_HOST_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, NULL, 0, NULL, 1, transferImageLayoutBarrier);

            // Copy the image
            VkBufferImageCopy region = {};
            region.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            region.imageSubresource.layerCount = 1;
            region.imageExtent.width = fontTexWidth;
            region.imageExtent.height = fontTexHeight;
            region.imageExtent.depth = 1;
            vkCmdCopyBufferToImage(copyCommand, stagingBuffer.buffer, _fontImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

            VkImageMemoryBarrier shaderReadImageLayoutBarrier[1] = {};
            shaderReadImageLayoutBarrier[0].sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
            shaderReadImageLayoutBarrier[0].srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
            shaderReadImageLayoutBarrier[0].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
            shaderReadImageLayoutBarrier[0].oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
            shaderReadImageLayoutBarrier[0].newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            shaderReadImageLayoutBarrier[0].srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            shaderReadImageLayoutBarrier[0].dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
            shaderReadImageLayoutBarrier[0].image = _fontImage;
            shaderReadImageLayoutBarrier[0].subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
            shaderReadImageLayoutBarrier[0].subresourceRange.levelCount = 1;
            shaderReadImageLayoutBarrier[0].subresourceRange.layerCount = 1;
            vkCmdPipelineBarrier(copyCommand, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, NULL, 0, NULL, 1, shaderReadImageLayoutBarrier);

            _context->ExecuteCommandBuffer(copyCommand);
            stagingBuffer.Release();
        }

        // Create Image Sampler
        {
            auto samplerCreateInfo = VkCreate::SamplerCreateInfo();
            if (vkCreateSampler(_context->device, &samplerCreateInfo, nullptr, &_fontSampler) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't create ImGui font sampler !");
            }
        }

        // Create the Descriptor Pool
        {
            VkDescriptorPoolSize poolSizes[] =
            {
                { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 }
            };

            auto descriptorPoolCreateInfo = VkCreate::DescriptorPoolCreateInfo(1, poolSizes, 2);
            if (vkCreateDescriptorPool(_context->device, &descriptorPoolCreateInfo, nullptr, &_descriptorPool) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't create ImGui descriptor pool !");
            }
        }

        // Create the Descriptor Set Layout
        {
            VkDescriptorSetLayoutBinding layoutBindings[] =
            {
                VkCreate::DescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0)
            };

            auto descriptorSetLayoutCreateInfo = VkCreate::DescriptorSetLayoutCreateInfo(layoutBindings, 1);
            if (vkCreateDescriptorSetLayout(_context->device, &descriptorSetLayoutCreateInfo, nullptr, &_descriptorSetLayout) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't create ImGui descriptor set layout !");
            }
        }

        // Allocate the descriptor set and bind it
        {
            auto descriptorSetAllocateInfo = VkCreate::DescriptorSetAllocateInfo(_descriptorPool, &_descriptorSetLayout, 1);
            if (vkAllocateDescriptorSets(_context->device, &descriptorSetAllocateInfo, &_descriptorSet) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't create ImGui descriptor set !");
            }

            auto fontImageDescriptor = VkCreate::DescriptorImageInfo(_fontSampler, _fontView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
            VkWriteDescriptorSet writeFontImageDescriptorSet[] =
            {
                VkCreate::WriteDescriptorSet(_descriptorSet, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 0, &fontImageDescriptor)
            };
            vkUpdateDescriptorSets(_context->device, 1, writeFontImageDescriptorSet, 0, nullptr);

        }

        // Create Pipeline Layout
        {
            auto pushConstantRange = VkCreate::PushConstantRange(VK_SHADER_STAGE_VERTEX_BIT, sizeof(PushConstants), 0);
            auto pipelineLayoutCreateInfo = VkCreate::PipelineLayoutCreateInfo(&_descriptorSetLayout, 1);
            pipelineLayoutCreateInfo.pushConstantRangeCount = 1;
            pipelineLayoutCreateInfo.pPushConstantRanges = &pushConstantRange;
            if (vkCreatePipelineLayout(_context->device, &pipelineLayoutCreateInfo, nullptr, &_pipelineLayout) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't create ImGui pipeline layout !");
            }
        }

        // Create the Graphics Pipeline
        {
            VkVertexInputBindingDescription vertexInputBindings[] = 
            {
                VkCreate::VertexInputBindingDescription(0, sizeof(ImDrawVert), VK_VERTEX_INPUT_RATE_VERTEX)
            };

            VkVertexInputAttributeDescription vertexInputAttributes[] =
            {
                VkCreate::VertexInputAttributeDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos)),
                VkCreate::VertexInputAttributeDescription(0, 1, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv)),
                VkCreate::VertexInputAttributeDescription(0, 2, VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col))
            };

            auto inputAssemblyState = VkCreate::PipelineInputAssemblyStateCreateInfo(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, 0, VK_FALSE);
            auto vertexInputState = VkCreate::PipelineVertexInputStateCreateInfo();
            vertexInputState.vertexBindingDescriptionCount = 1;
            vertexInputState.vertexAttributeDescriptionCount = 3;
            vertexInputState.pVertexBindingDescriptions = vertexInputBindings;
            vertexInputState.pVertexAttributeDescriptions = vertexInputAttributes;

            auto rasterizationState = VkCreate::PipelineRasterizationStateCreateInfo(VK_POLYGON_MODE_FILL, VK_CULL_MODE_NONE, VK_FRONT_FACE_COUNTER_CLOCKWISE);
            auto blendAttachmentState = VkCreate::PipelineColorBlendAttachmentState(VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT, VK_TRUE);
            blendAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
            blendAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            blendAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
            blendAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
            blendAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
            blendAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;

            auto colorBlendState = VkCreate::PipelineColorBlendStateCreateInfo(1, &blendAttachmentState);
            auto depthStencilState = VkCreate::PipelineDepthStencilStateCreateInfo(VK_FALSE, VK_FALSE, VK_COMPARE_OP_LESS_OR_EQUAL);
            auto viewportState = VkCreate::PipelineViewportStateCreateInfo(1, 1, 0);
            auto multisampleState = VkCreate::PipelineMultisampleStateCreateInfo(VK_SAMPLE_COUNT_1_BIT);

            VkDynamicState dynamicStates[] =
            {
                VK_DYNAMIC_STATE_VIEWPORT,
                VK_DYNAMIC_STATE_SCISSOR
            };

            auto dynamicState = VkCreate::PipelineDynamicStateCreateInfo(dynamicStates, 2);

            // Create shader modules
            auto vertexShaderCreateInfo = VkCreate::ShaderModuleCreateInfo(sizeof(__glsl_shader_vert_spv), __glsl_shader_vert_spv);
            auto fragmentShaderCreateInfo = VkCreate::ShaderModuleCreateInfo(sizeof(__glsl_shader_frag_spv), __glsl_shader_frag_spv);
            VkShaderModule vertexShaderModule = VK_NULL_HANDLE;
            VkShaderModule fragmentShaderModule = VK_NULL_HANDLE;

            if (vkCreateShaderModule(_context->device, &vertexShaderCreateInfo, nullptr, &vertexShaderModule) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't create ImGui vertex shader !");
            }

            if (vkCreateShaderModule(_context->device, &fragmentShaderCreateInfo, nullptr, &fragmentShaderModule) != VK_SUCCESS)
            {
                throw std::runtime_error("Couldn't create ImGui fragment shader !");
            }

            VkPipelineShaderStageCreateInfo shaderStages[] =
            {
                VkCreate::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_VERTEX_BIT, "main", vertexShaderModule),
                VkCreate::PipelineShaderStageCreateInfo(VK_SHADER_STAGE_FRAGMENT_BIT, "main", fragmentShaderModule)
            };

            auto pipelineCreateInfo = VkCreate::PipelineCreateInfo(_pipelineLayout, renderPass);
            pipelineCreateInfo.pInputAssemblyState = &inputAssemblyState;
            pipelineCreateInfo.pVertexInputState = &vertexInputState;
            pipelineCreateInfo.pRasterizationState = &rasterizationState;
            pipelineCreateInfo.pColorBlendState = &colorBlendState;
            pipelineCreateInfo.pMultisampleState = &multisampleState;
            pipelineCreateInfo.pViewportState = &viewportState;
            pipelineCreateInfo.pDepthStencilState = &depthStencilState;
            pipelineCreateInfo.pDynamicState = &dynamicState;
            pipelineCreateInfo.stageCount = 2;
            pipelineCreateInfo.pStages = shaderStages;

            if (vkCreateGraphicsPipelines(_context->device, nullptr, 1, &pipelineCreateInfo, nullptr, &_pipeline))
            {
                throw std::runtime_error("Couldn't create ImGui pipeline !");
            }

            vkDestroyShaderModule(_context->device, vertexShaderModule, nullptr);
            vkDestroyShaderModule(_context->device, fragmentShaderModule, nullptr);
        }
    }

    void VulkanImGui::Release()
    {
        _vertexBuffer.Release();
        _indexBuffer.Release();

        vkDestroyImage(_context->device, _fontImage, nullptr);
        vkDestroyImageView(_context->device, _fontView, nullptr);
        vkFreeMemory(_context->device, _fontMemory, nullptr);
        vkDestroySampler(_context->device, _fontSampler, nullptr);
        vkDestroyPipeline(_context->device, _pipeline, nullptr);
        vkDestroyPipelineLayout(_context->device, _pipelineLayout, nullptr);
        vkDestroyDescriptorPool(_context->device, _descriptorPool, nullptr);
        vkDestroyDescriptorSetLayout(_context->device, _descriptorSetLayout, nullptr);
    }
}