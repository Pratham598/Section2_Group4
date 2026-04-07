# Traffic Monitoring System - Test Cases

Below are 5 test cases for the C++ traffic monitoring system based on the `Packet`, `Server`, and `Client` classes.

| Test Case Name | Input | Expected Output | Actual Output | Result (Pass/Fail) |
| :--- | :--- | :--- | :--- | :--- |
| **Packet Encoding** | `type=1`, `seqNum=100`, `data="Test"` passed to `Packet` constructor. Then calling `encode()`. | Encoded string matching format (e.g., `"1|100|4|Test"`) | Successfully encoded string `"1|100|4|Test"` | **Pass** |
| **Packet Decoding** | Calling `decode("2|50|6|Status")` | Packet sets `packetType=2`, `seqNum=50`, `payload="Status"` | Object internal variables match expected values | **Pass** |
| **Valid Server Auth** | `authenticateUser("admin", "password123")` | Returns `true` and sets `isAuthenticated` to true | `true` | **Pass** |
| **Invalid Server Auth**| `authenticateUser("hacker", "wrongpass")` | Returns `false` and blocks further monitoring requests | `false` | **Pass** |
| **Client Camera Request** | Client calling `sendRequest("CAMERA_1")` when authenticated | Server transitions to `MONITORING` and sends confirmation | Response `"CAMERA_1_STREAM_START"` received by Client | **Pass** |
