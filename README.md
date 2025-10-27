# ni-grpc-sideband-example
Lightweight example for grpc-sideband client using nidaqmx.

## Server-side gRPC Configuration

1. Edit the server configuration file (e.g., `server_config.yaml` or `config.json`) as follows:

```bash
{
    "address": "[::]",
    "port": 31763,
    "sideband_address": "<server_ip>",
    "sideband_port": 50055,
    "security" : {
       "server_cert": "",
       "server_key": "",
       "root_cert": ""
    },
"feature_toggles": {
  "moniker_streaming": true,
  "moniker_streaming_sideband_support": true
}
```

2. Verify that port, credentials, and other parameters are correctly set. Take on consideration that the `sideband_address` should be the one one where the server is running.
3. Start the server.

```bash
~/<path_to_server_build># ./ni_grpc_device_server
```

You should receive something like this:

```bash
Using server configuration from <path>/grpc-device/cmake/build/server_config.json
Server listening on port 31763
Security is configured with insecure credentials.
Listening for sideband sockets at: <server_ip>:50055
```

## Build the application

- Go to the correct path where you want to build the client:

```bash
cd <path_to_cloned_repository>/examples/nidaqmx/moniker-streaming/
```

- Build the application:

```bash
mkdir build
cd build
cmake -DINCLUDE_SIDEBAND_RDMA=OFF ..
cmake --build .
```

### Running the Tool

Usage example:

```bash
./MonikerStreamingClient <server-ip> <server-port> <read-device> <write-device> <number-channels>
```

Where:

- `<server-ip>`: The IP address where your gRPC server is running.
    
    Example: `127.0.0.1` or `192.168.1.100`

- `<server-port>`: The port number on which your gRPC server is listening.

    Example: `31763`

- `<read-device>`: The identifier or name of the NI DAQmx device from which to read data.

    Example: `Dev1`

- `<write-device>`: The identifier or name of the NI DAQmx device to which to write data.

    Example: `Dev2`

- `<number-channels>`: The number of channels yo want to read and write.

    Example: `2` (for two channels)

