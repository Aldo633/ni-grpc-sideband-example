/*********************************************************************
* Acquire data continuously from an Integer indicator in an FPGA bitfile, and write data to a control. 
* In order to use this example, you must first create an FPGA bitfile with the NI FPGA Module.
* The gRPC API is built from the C API. NI-DAQmx documentation is installed with the driver at:
*
*
* Getting Started:
*
* To run this example, install FPGA Interface C API on a Windows Machine, and use it to generate the supporting header files for the bitfile
*   https://knowledge.ni.com/KnowledgeArticleDetails?id=kA03q000000x5DNCAY&l=en-US
*
* For instructions on how to use protoc to generate gRPC client interfaces, see our "Creating a gRPC
* Client" wiki page:
*   https://github.com/ni/grpc-device/wiki/Creating-a-gRPC-Client
*
*
*
* Build:
*
*   > mkdir build
*   > cd build
*   > cmake ..
*   > cmake --build .
*
* Running from command line:
*
* Server machine's IP address, port number, and physical channel name can be passed as separate
* command line arguments.
*   > MonikerStreamingClient <server_address> <port_number> <fpga_resource>
* If they are not passed in as command line arguments, then by default the server address will be
* "localhost:31763" and "RIO0" as the FPGA resource.
*********************************************************************/

#include <iostream>
#include <sstream>
#include <grpcpp/grpcpp.h>
#include <sideband_grpc.h>


#include "nifpga.grpc.pb.h"
#include "data_moniker.grpc.pb.h"

using namespace nifpga_grpc;
using StubPtr = std::unique_ptr<NiFpga::Stub>;


std::string SERVER_ADDRESS = "localhost";
std::string SERVER_PORT = "31763";
std::string FPGA_RESOURCE = "RIO0";
std::string BITFILE_PATH = "/home/admin/PXIe-7842R.lvbitx";
std::string NI_FPGA_EXAMPLE_SIGNATURE = "11F1FF1D11F1FF1F1F111FF11F11F111";
int EXAMPLE_ARRAY_CONTROL = 0x10004;
int EXAMPLE_ARRAY_CONTROL_SIZE = 4;

int NUM_ITERATIONS = 5;


class grpc_driver_error : public std::runtime_error {
 private:
  ::grpc::StatusCode code_;
  std::multimap<std::string, std::string> trailers_;

 public:
  grpc_driver_error(const std::string& message, ::grpc::StatusCode code, const std::multimap<grpc::string_ref, grpc::string_ref>& trailers)
    : std::runtime_error(message), code_(code)
  {
    for (const auto& trailer : trailers) {
      trailers_.emplace(
        std::string(trailer.first.data(), trailer.first.length()),
        std::string(trailer.second.data(), trailer.second.length()));
    }
  }

  ::grpc::StatusCode StatusCode() const
  {
    return code_;
  }

  const std::multimap<std::string, std::string>& Trailers() const
  {
    return trailers_;
  }
};

inline void raise_if_error(const ::grpc::Status& status, const ::grpc::ClientContext& context)
{
  if (!status.ok()) {
    throw grpc_driver_error(status.error_message(), status.error_code(), context.GetServerTrailingMetadata());
  }
}



void print_array(const MonikerReadArrayI64Response& data)
{
  std::cout << "Array Size: " << data.array().size() << " ";
  std::cout << "[";
    for (int i = 0; i < data.array().size(); i++) {
    std::cout << data.array().Get(i) << " ";
  }
  std::cout << "]" << std::endl;
}

::nidevice_grpc::Session create_and_configure_fpga_task(NiFpga::Stub &client, const std::string &FPGA_RESOURCE)
{

  
    
    ::grpc::ClientContext fpga_open_context;
    auto fpga_open_request = OpenRequest{};
    fpga_open_request.set_session_name("my fpga session");
    fpga_open_request.set_bitfile(BITFILE_PATH);
    fpga_open_request.set_signature(NI_FPGA_EXAMPLE_SIGNATURE);
    fpga_open_request.set_resource(FPGA_RESOURCE);
    fpga_open_request.set_attribute_mapped(OpenAttribute::OPEN_ATTRIBUTE_NO_RUN);
    auto fpga_open_response = OpenResponse{};
    raise_if_error(
      client.Open(&fpga_open_context, fpga_open_request, &fpga_open_response),
      fpga_open_context);
    auto fpga_task = fpga_open_response.session();

    ::grpc::ClientContext fpga_run_context;
    auto fpga_run_request = RunRequest{};
    fpga_run_request.mutable_session()->CopyFrom(fpga_task);
    auto fpga_run_response = RunResponse{};
    raise_if_error(
      client.Run(&fpga_run_context, fpga_run_request, &fpga_run_response),
      fpga_run_context);    

  return fpga_task;
}

int main(int argc, char **argv)
{
  if (argc >= 2) {
    SERVER_ADDRESS = argv[1];
  }
  if (argc >= 3) {
    SERVER_PORT = argv[2];
  }
  if (argc >= 4) {
    FPGA_RESOURCE = argv[3];
  }

  auto target_str = SERVER_ADDRESS + ":" + SERVER_PORT;

  std::cout << "Configuration:\n";
  std::cout << "  Server: " << target_str << "\n";
  


  // Create gRPC channel - Connect to server
  auto channel = grpc::CreateChannel(target_str, grpc::InsecureChannelCredentials());
  //NiDAQmx::Stub client(channel);
  NiFpga::Stub fpga_client(channel);
  ni::data_monikers::DataMoniker::Stub moniker_service(channel);

  try {


    std::vector<int64_t> write_data_i64(8, 1);
    std::cout << "Set up FPGA Bitfile" << std::endl;
    auto fpga_task = create_and_configure_fpga_task(fpga_client, FPGA_RESOURCE);

    



    //ni::data_monikers::Moniker* fpga_write_array_moniker = new ni::data_monikers::Moniker();
    /*fpga_client.BeginWriteArrayI64(fpga_session,
                                   NiFpga_FPGA_Read_Write_Array_Loop_ControlArrayI64_write_i64,
                                   NiFpga_FPGA_Read_Write_Array_Loop_ControlArrayI64Size_write_i64,
                                   fpga_write_array_moniker);*/

    //ni::data_monikers::Moniker* fpga_write_moniker = new ni::data_monikers::Moniker();
    /*fpga_client.BeginWriteI64(fpga_session,
                            NiFpga_FPGA_Read_Write_Array_Loop_ControlI64_add_i64,
                            fpga_write_moniker);

    /*ni::data_monikers::Moniker* fpga_read_moniker = new ni::data_monikers::Moniker();
    fpga_client.BeginReadArrayI64(fpga_session,
                                  NiFpga_FPGA_Read_Write_Array_Loop_IndicatorArrayI64_read_i64,
                                  NiFpga_FPGA_Read_Write_Array_Loop_IndicatorArrayI64Size_read_i64,
                                  fpga_read_moniker);*/

    //std::cout << "FPGA setup complete" << std::endl;


    // Setup the read moniker
    std::cout << "Set up Read Array Moniker" << std::endl;
    ::grpc::ClientContext begin_read_array_i64_context;
    auto begin_read_array_i64_request = BeginReadArrayI64Request{};
    begin_read_array_i64_request.mutable_session()->CopyFrom(fpga_task);
    begin_read_array_i64_request.set_indicator(EXAMPLE_ARRAY_CONTROL);
    begin_read_array_i64_request.set_size(EXAMPLE_ARRAY_CONTROL_SIZE);
    auto begin_read_array_i64_response = BeginReadArrayI64Response{};
    raise_if_error(
      fpga_client.BeginReadArrayI64(&begin_read_array_i64_context, begin_read_array_i64_request, &begin_read_array_i64_response),
      begin_read_array_i64_context);
    auto fpga_read_array_i64moniker = new ni::data_monikers::Moniker(begin_read_array_i64_response.moniker());
    


    // Setup the write moniker
    std::cout << "Setup Write Moniker" << std::endl;
    ::grpc::ClientContext begin_write_array_i64_context;
    auto begin_write_array_i64_request = BeginWriteArrayI64Request{};
    begin_write_array_i64_request.mutable_session()->CopyFrom(fpga_task);
    begin_write_array_i64_request.set_control(EXAMPLE_ARRAY_CONTROL);
    auto begin_write_array_i64_response = BeginWriteArrayI64Response{};
    raise_if_error(
      fpga_client.BeginWriteArrayI64(&begin_write_array_i64_context, begin_write_array_i64_request, &begin_write_array_i64_response),
      begin_write_array_i64_context);
    auto fpga_write_array_i64moniker = new ni::data_monikers::Moniker(begin_write_array_i64_response.moniker());

    //Setup Sideband Stream
    grpc::ClientContext moniker_context;
    ni::data_monikers::BeginMonikerSidebandStreamRequest sideband_request;
    ni::data_monikers::BeginMonikerSidebandStreamResponse sideband_response;
    sideband_request.set_strategy(ni::data_monikers::SidebandStrategy::SOCKETS_LOW_LATENCY);
    sideband_request.mutable_monikers()->mutable_read_monikers()->AddAllocated(fpga_read_array_i64moniker);
    sideband_request.mutable_monikers()->mutable_write_monikers()->AddAllocated(fpga_write_array_i64moniker);
    auto write_stream = moniker_service.BeginSidebandStream(&moniker_context, sideband_request, &sideband_response);
    if(!write_stream.ok()) {
       std::cout << "ERROR BeginSidebandStream: (" << write_stream.error_code() << ") " << write_stream.error_message() << std::endl;
    }
    auto sideband_token = InitClientSidebandData(sideband_response);
    std::cout << "InitClientSidebandData complete with token " << sideband_token << std::endl;
    

    //Read data and write data
    for (int i = 0; i < NUM_ITERATIONS; i++) {
      ni::data_monikers::MonikerReadResponse read_data_result;
      nifpga_grpc::MonikerWriteArrayI64Request write_values_array_i64;
      ni::data_monikers::SidebandWriteRequest sideband_request;

      write_values_array_i64.mutable_array()->Add(write_data_i64.begin(), write_data_i64.end());
      sideband_request.mutable_values()->add_values()->PackFrom(write_values_array_i64);

      WriteSidebandMessage(sideband_token, sideband_request);
       std::cout << "Write Sideband Message done" << std::endl;

      MonikerReadArrayI64Response read_array_i64_response;
      ni::data_monikers::SidebandReadResponse read_result;
      ReadSidebandMessage(sideband_token, &read_result);
      auto status = read_result.values().values(0).UnpackTo(&read_array_i64_response);

        std::cout << "Status of Unpack" << status << std::endl;
  
      std::cout << "Read data..." << std::endl;
      print_array(read_array_i64_response);
      
      }

    ni::data_monikers::SidebandWriteRequest cancel_request;
    cancel_request.set_cancel(true);
    WriteSidebandMessage(sideband_token, cancel_request);
    CloseSidebandData(sideband_token);
    

    std::cout << "Cleaning up." << std::endl;

    ::grpc::ClientContext fpga_close_context;
    CloseRequest close_request;
    close_request.mutable_session()->CopyFrom(fpga_task);
    CloseResponse close_response;
    raise_if_error(
      fpga_client.Close(&fpga_close_context, close_request, &close_response),
      fpga_close_context);

   
  }
  catch (const grpc_driver_error& e) {
    std::string error_message = e.what();

    for (const auto& entry : e.Trailers()) {
      if (entry.first == "ni-error") {
        error_message += "\nError status: " + entry.second;
      }
    }

    if (e.StatusCode() == grpc::StatusCode::UNAVAILABLE) {
      error_message = "Failed to connect to server on " + SERVER_ADDRESS + ":" + SERVER_PORT;
    }
    else if (e.StatusCode() == grpc::StatusCode::UNIMPLEMENTED) {
      error_message = "The operation is not implemented or is not supported/enabled in this service";
    }

    std::cout << "Exception: " << error_message << std::endl;
  }
}