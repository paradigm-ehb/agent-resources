# list services
grpcurl -plaintext localhost:5000 list resources.v3.ResourcesService

# cpu service
grpcurl -plaintext -v \
  -d '{}' \
  -emit-defaults \
  -format json \
  localhost:5000 \
  resources.v3.ResourcesService/GetCpu

# ram service
grpcurl -plaintext -v \
  -d '{}' \
  -emit-defaults \
  -format json \
  localhost:5000 \
  resources.v3.ResourcesService/GetMemory

