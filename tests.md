# Traffic Monitoring System - Test Cases

Below are the validated tests specifically covering the login, state transitions, snapshots, and error handling of the Server.

| Test Case | Input / Action | Expected Output | Actual Output | Result |
| :--- | :--- | :--- | :--- | :--- |
| **Login Success** | `authenticateUser("admin", "password123")` | Returns `true`; server `isAuthenticated` becomes true | `true` | **Pass** |
| **Login Failure** | `authenticateUser("hacker", "badpass")` | Returns `false`; blocks all subsequent packet requests | `false` | **Pass** |
| **State Transition (Start)** | `START_MONITOR` packet sent while IDLE | State becomes `MONITORING`; Returns `"Server state changed to MONITORING."` | Successfully transitioned state and returned confirmation | **Pass** |
| **State Transition (Stop)** | `STOP_MONITOR` packet sent while MONITORING | State reverts to `IDLE`; Returns `"Server state changed to IDLE."` | Successfully transitioned state and returned confirmation | **Pass** |
| **Snapshot Request** | `GET_SNAPSHOT` packet sent while MONITORING | Image separated into chunks (`CHUNK:1/3:`, etc.) sent across 3 packets | 3 valid packets generated representing 1.5MB mocked image | **Pass** |
| **Invalid State Command** | `GET_SNAPSHOT` packet sent while IDLE | Returns error rejecting the action since cameras aren't active | `"Error: Rejected invalid action 'GET_SNAPSHOT' in IDLE state."` | **Pass** |
| **Invalid Unknown Command** | `HACK_SYSTEM` packet sent | Returns unknown request error | `"Error: Unknown request 'HACK_SYSTEM'."` | **Pass** |
