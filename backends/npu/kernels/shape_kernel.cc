// Copyright (c) 2022 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "kernels/funcs/npu_funcs.h"
#include "kernels/funcs/npu_op_runner.h"

namespace custom_kernel {

template <typename T, typename Context>
void ShapeKernel(const Context& dev_ctx,
                 const phi::DenseTensor& input,
                 phi::DenseTensor* out) {
  out->Resize({input.dims().size()});
  dev_ctx.template Alloc<int32_t>(out);

  // The output data type defaults to int32.
  auto stream = dev_ctx.stream();
  NpuOpRunner runner;
  auto dst_dtype = ConvertToNpuDtype(phi::DataType::INT32);
  runner.SetType("Shape").AddInput(input).AddOutput(*out).AddAttr(
      "dtype", static_cast<int>(dst_dtype));
  runner.Run(stream);
}

}  // namespace custom_kernel

PD_REGISTER_PLUGIN_KERNEL(shape,
                          ascend,
                          ALL_LAYOUT,
                          custom_kernel::ShapeKernel,
                          bool,
                          int,
                          int8_t,
                          uint8_t,
                          int64_t,
                          float,
                          double,
                          phi::dtype::float16) {
  kernel->InputAt(0).SetBackend(phi::Backend::ALL_BACKEND);
}
