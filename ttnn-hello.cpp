#include <cstddef>
#include <ttnn/core.hpp>
#include <ttnn/operations/eltwise/unary/unary.hpp>
#include <ttnn/device.hpp>
#include <ttnn/operations/data_movement/tilize_with_val_padding/tilize_with_val_padding.hpp>

#include <vector>
#include <iostream>

ttnn::IDevice* device = nullptr;

void print_tensor(const tt::tt_metal::Tensor& tensor)
{
    // IMPORTANT. This function prints the tensor data assuming the tensor is in ROW_MAJOR layout
    // but we are using TILE layout. The printed format WILL NOT be correct. But good enough for a demo

    // Get the shape of the tensor
    auto shape = tensor.logical_shape();
    // compyte the size of the tensor
    size_t size = 1;
    for(size_t i = 0; i < shape.size(); i++)
        size *= shape[i];

    // prepare a buffer to copy the tensor data to the host
    std::vector<bfloat16> data(size);
    tt::tt_metal::memcpy(device->command_queue(), data.data(), tensor);

    // print the data
    for(size_t i = 0; i < shape[0]; i++)
    {
        for(size_t j = 0; j < shape[1]; j++)
        {
            for(size_t k = 0; k < shape[2]; k++)
            {
                for(size_t l = 0; l < shape[3]; l++)
                {
                    std::cout << data[i * shape[1] * shape[2] * shape[3] + j * shape[2] * shape[3] + k * shape[3] + l].to_float() << " ";
                }
                std::cout << std::endl;
            }
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
}

int main()
{
    const size_t tensor_width = 32;
    const size_t tensor_height = 32;

    // tell TTNN that we want to use the first device available
    device = &ttnn::device::open_device(0);

    std::cout << "Creating a tensor with bfloat16 data type" << std::endl;
    // TTNN wants us to explicitly specify if the tensor owns the buffer or not. if not, we need to make dman sure that the buffer is not deallocated before the tensor
    auto buffer = tt::tt_metal::owned_buffer::create(
        create_random_vector_of_bfloat16_native(
            // In number of bytes. so 2 bytes per bfloat16 element
            tensor_width * tensor_height * 2
            //  max = 2, offset = -1, seed = 42. Effectively, the range is [-1, 1]. I know, weird API
            , 2, 42, -1));
    // Now we create a tensor with the buffer we just created
    auto x = tt::tt_metal::Tensor(
        // Let the tensor take ownership of the buffer
        tt::tt_metal::OwnedStorage{std::move(buffer)},
        // IMPORTANT: SHAPE MUST BE 4D ELSE EVERYTHING WILL BREAK during the PAD operation
        ttnn::Shape({1, 1,tensor_width, tensor_height}),
        // The data type of the tensor
        tt::tt_metal::DataType::BFLOAT16,
        // The layout of the tensor. We don't care about the layout in this demo. But the valid options are TILE and ROW_MAJOR
        // Where TILE is the processor native layout and ROW_MAJOR mostly have to be converted to TILE before processing
        tt::tt_metal::Layout::TILE);
    // Once created, the tensor "on host" and we must move it to the device to perform operations on it
    x = x.to_device(device);

    // Print the tensor to see what it looks like
    std::cout << "Tensot x:\n";
    print_tensor(x);

    // Perform the sin(x) operation on the tensor
    std::cout << "Performing operation on the tensor" << std::endl;
    auto y = ttnn::sin(x);
    // You can try other operations like relu, sigmoid and what not. Or adding two tensors!
    // auto y = ttnn::add(x, x);

    // Print the result
    std::cout << "Tensot y:\n";
    print_tensor(y);

    // Remember to close the device when you are done
    std::cout << "Done. Shutting down" << std::endl;
    device->close();
}
