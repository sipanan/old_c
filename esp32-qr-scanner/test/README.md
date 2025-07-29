# ESP32-CAM QR Scanner Test Suite

## Unit Tests

This directory contains comprehensive unit tests for all system components.

### Test Structure
```
test/
├── test_camera_manager.py     # Camera interface tests
├── test_qr_detector.py        # QR detection algorithm tests
├── test_web_server.py         # HTTP server and API tests
├── test_wifi_manager.py       # WiFi connectivity tests
├── test_config_manager.py     # Configuration persistence tests
├── test_utils.py              # Utility function tests
├── test_integration.py        # End-to-end integration tests
├── test_performance.py        # Performance and load tests
├── test_memory.py             # Memory leak and usage tests
└── requirements.txt           # Python test dependencies
```

### Running Tests

#### Prerequisites
```bash
pip install -r requirements.txt
```

#### Execute Test Suite
```bash
# Run all tests
python -m pytest

# Run specific test file
python -m pytest test_camera_manager.py -v

# Run with coverage
python -m pytest --cov=../main --cov-report=html

# Performance tests (requires hardware)
python -m pytest test_performance.py --hardware
```

### Test Categories

#### Unit Tests
- **Camera Manager**: Frame capture, buffer management, sensor control
- **QR Detector**: Algorithm accuracy, error correction, content parsing
- **Web Server**: HTTP endpoints, authentication, response validation
- **WiFi Manager**: Connection handling, error recovery, signal monitoring
- **Config Manager**: Data persistence, encryption, migration

#### Integration Tests
- **Full System**: Camera → QR Detection → Web Interface workflow
- **Multi-threading**: Concurrent operation of all components
- **Error Recovery**: Fault injection and recovery validation
- **Memory Management**: PSRAM/SRAM allocation and cleanup

#### Performance Tests
- **Throughput**: Frame processing rate and QR detection speed
- **Latency**: End-to-end response time measurements
- **Memory**: Usage patterns and leak detection
- **Power**: Consumption monitoring and optimization
- **Stability**: Long-term operation and reliability

### Hardware-in-Loop Testing

#### Required Hardware
- ESP32-CAM development board
- USB-Serial adapter for programming
- Test QR codes (various sizes and types)
- Controlled lighting environment
- Network infrastructure

#### Test Scenarios
1. **Detection Accuracy**: Various QR code types and conditions
2. **Environmental Stress**: Temperature, humidity, vibration
3. **Network Resilience**: Connection drops, packet loss, latency
4. **Power Cycling**: Restart behavior and state recovery
5. **Concurrent Users**: Multiple simultaneous web connections

### Continuous Integration

#### GitHub Actions Workflow
```yaml
name: ESP32-CAM QR Scanner CI

on: [push, pull_request]

jobs:
  test:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v2
    - name: Setup Python
      uses: actions/setup-python@v2
      with:
        python-version: 3.9
    - name: Install dependencies
      run: pip install -r test/requirements.txt
    - name: Run unit tests
      run: python -m pytest test/ --cov=main
    - name: Upload coverage
      uses: codecov/codecov-action@v1
```

### Test Results and Reports

#### Coverage Targets
- **Unit Test Coverage**: >95%
- **Integration Coverage**: >90%
- **API Endpoint Coverage**: 100%
- **Error Path Coverage**: >85%

#### Performance Benchmarks
- **QR Detection Rate**: >99.5% for standard codes
- **Frame Processing**: <100ms average
- **Memory Usage**: <80% SRAM utilization
- **Response Time**: <50ms API responses
- **Uptime**: 99.9% reliability target

### Test Data and Fixtures

#### QR Code Test Patterns
- **Standard patterns**: URL, WiFi, vCard, SMS formats
- **Stress patterns**: Maximum data capacity, error correction
- **Edge cases**: Rotated, damaged, partial visibility
- **Security patterns**: Encrypted content, malformed data

#### Mock Hardware
For development without physical hardware:
- Camera frame simulation with configurable patterns
- Network interface mocking for offline testing
- Memory allocation simulation for resource testing
- Timer mocking for performance measurement

---

This test suite ensures the ESP32-CAM QR scanner meets enterprise-grade quality and reliability standards required for mission-critical applications.