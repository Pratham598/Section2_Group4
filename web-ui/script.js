// DOM Elements
const loginSection = document.getElementById('login-section');
const registerSection = document.getElementById('register-section');
const forgotPasswordSection = document.getElementById('forgot-password-section');
const monitoringSection = document.getElementById('monitoring-section');
const modalOverlay = document.getElementById('modal-overlay');

const showRegisterLink = document.getElementById('show-register-link');
const showForgotLink = document.getElementById('show-forgot-link');
const regCloseBtn = document.getElementById('reg-close');
const forgotCloseBtn = document.getElementById('forgot-close');
const registerSubmitBtn = document.getElementById('register-submit-btn');
const regUsernameInput = document.getElementById('reg-username');
const regPasswordInput = document.getElementById('reg-password');
const regConfirmInput = document.getElementById('reg-confirm');
const registerMessage = document.getElementById('register-message');

const forgotSubmitBtn = document.getElementById('forgot-submit-btn');
const forgotInput = document.getElementById('forgot-input');
const forgotMessage = document.getElementById('forgot-message');

const loginBtn = document.getElementById('login-btn');
const usernameInput = document.getElementById('username');
const passwordInput = document.getElementById('password');

const startBtn = document.getElementById('start-btn');
const stopBtn = document.getElementById('stop-btn');
const snapshotBtn = document.getElementById('snapshot-btn');

const cameraStatus = document.getElementById('camera-status');
const logsContainer = document.getElementById('console-logs');

const snapshotPreviewContainer = document.getElementById('snapshot-preview-container');
const snapshotStatusText = document.getElementById('snapshot-status-text');
const snapshotBox = document.getElementById('snapshot-box');

const systemInfo = document.getElementById('system-info');
const displayUser = document.getElementById('display-user');

// State Management
const STATE_IDLE = 'IDLE';
const STATE_MONITORING = 'MONITORING';
let currentState = STATE_IDLE; // Default state

// Helper to add logs to the output area
function addLog(message, type = 'info') {
    const timestamp = new Date().toLocaleTimeString();
    const logElement = document.createElement('div');
    logElement.className = `log-entry ${type}`;
    logElement.textContent = `[${timestamp}] ${message}`;
    logsContainer.appendChild(logElement);
    
    // Auto-scroll to bottom
    logsContainer.scrollTop = logsContainer.scrollHeight;
}

// Update the UI state indicator
function updateUIState() {
    if (currentState === STATE_MONITORING) {
        cameraStatus.className = 'status-indicator online';
        cameraStatus.querySelector('.status-text').textContent = `State: ${currentState}`;
        // Update buttons for MONITORING state
        startBtn.disabled = true;
        stopBtn.disabled = false;
        snapshotBtn.disabled = false;
    } else {
        cameraStatus.className = 'status-indicator offline';
        cameraStatus.querySelector('.status-text').textContent = `State: ${currentState}`;
        // Update buttons for IDLE state
        startBtn.disabled = false;
        stopBtn.disabled = true;
        snapshotBtn.disabled = true;
        
        // Hide snapshot preview on idle
        snapshotPreviewContainer.classList.add('hidden');
    }
}

// Handle Login
loginBtn.addEventListener('click', () => {
    const username = usernameInput.value.trim();
    const password = passwordInput.value.trim();

    if (username && password) {
        addLog('Login successful', 'success');
        loginSection.classList.add('hidden');
        monitoringSection.classList.remove('hidden');
        
        displayUser.textContent = `User: ${username}`;
        systemInfo.classList.remove('hidden');
        
        // Initialize the UI with default state
        updateUIState();
    } else {
        alert('Please enter both username and password.');
    }
});

// Handle enter key in password input
passwordInput.addEventListener('keypress', (e) => {
    if (e.key === 'Enter') {
        loginBtn.click();
    }
});

// Show Registration Form
showRegisterLink.addEventListener('click', (e) => {
    e.preventDefault();
    modalOverlay.classList.remove('hidden');
    registerSection.classList.remove('hidden');
    registerMessage.style.display = 'none';
    regUsernameInput.value = '';
    regPasswordInput.value = '';
    regConfirmInput.value = '';
});

// Close Registration Form via X
regCloseBtn.addEventListener('click', () => {
    modalOverlay.classList.add('hidden');
    registerSection.classList.add('hidden');
});

// Handle global close via overlay
modalOverlay.addEventListener('click', () => {
    modalOverlay.classList.add('hidden');
    registerSection.classList.add('hidden');
    forgotPasswordSection.classList.add('hidden');
});

// Handle Registration
registerSubmitBtn.addEventListener('click', () => {
    const user = regUsernameInput.value.trim();
    const pass = regPasswordInput.value.trim();
    const conf = regConfirmInput.value.trim();

    if (!user || !pass || !conf) {
        registerMessage.style.display = 'block';
        registerMessage.style.color = 'var(--error-color)';
        registerMessage.textContent = 'Please fill in all fields.';
        return;
    }
    
    if (pass !== conf) {
        registerMessage.style.display = 'block';
        registerMessage.style.color = 'var(--error-color)';
        registerMessage.textContent = 'Passwords do not match.';
        return;
    }

    // Success state
    registerMessage.style.display = 'block';
    registerMessage.style.color = '#28a745';
    registerMessage.textContent = 'Account created successfully';
    
    // Automatically hide modal without disturbing base view
    setTimeout(() => {
        registerSection.classList.add('hidden');
        modalOverlay.classList.add('hidden');
    }, 1500);
});

// Show Forgot Password Form
showForgotLink.addEventListener('click', (e) => {
    e.preventDefault();
    modalOverlay.classList.remove('hidden');
    forgotPasswordSection.classList.remove('hidden');
    forgotMessage.style.display = 'none';
    forgotInput.value = '';
});

// Close Forgot Form via X
forgotCloseBtn.addEventListener('click', () => {
    modalOverlay.classList.add('hidden');
    forgotPasswordSection.classList.add('hidden');
});

// Handle Reset Password
forgotSubmitBtn.addEventListener('click', () => {
    const inputVal = forgotInput.value.trim();

    if (!inputVal) {
        forgotMessage.style.display = 'block';
        forgotMessage.style.color = 'var(--error-color)';
        forgotMessage.textContent = 'Please enter your email or username.';
        return;
    }

    forgotMessage.style.display = 'block';
    forgotMessage.style.color = '#28a745';
    forgotMessage.textContent = 'Password reset link sent';
    
    setTimeout(() => {
        forgotPasswordSection.classList.add('hidden');
        modalOverlay.classList.add('hidden');
    }, 1500);
});

// Handle Start Monitoring
startBtn.addEventListener('click', () => {
    // Only allow if IDLE
    if (currentState !== STATE_IDLE) {
        addLog('Error: Invalid action. System is already in MONITORING state.', 'error');
        return;
    }
    
    currentState = STATE_MONITORING;
    updateUIState();
    addLog('Server state changed to MONITORING', 'info');
});

// Handle Stop Monitoring
stopBtn.addEventListener('click', () => {
    // Only allow if MONITORING
    if (currentState !== STATE_MONITORING) {
        addLog('Error: Invalid action. System is already in IDLE state.', 'error');
        return;
    }
    
    currentState = STATE_IDLE;
    updateUIState();
    addLog('Server state changed to IDLE', 'info');
});

// Handle Snapshot
snapshotBtn.addEventListener('click', () => {
    // Only allow if MONITORING
    if (currentState !== STATE_MONITORING) {
        addLog('Error: Cannot take a snapshot while system is in IDLE state.', 'error');
        return;
    }
    
    snapshotBtn.disabled = true;
    
    snapshotPreviewContainer.classList.remove('hidden');
    snapshotStatusText.textContent = 'Receiving chunks...';
    snapshotBox.style.backgroundColor = '#fafafa';
    
    addLog('Receiving chunks...', 'info');
    
    setTimeout(() => {
        addLog('Snapshot fully assembled', 'success');
        
        snapshotStatusText.textContent = 'Snapshot fully assembled';
        
        // Inject physical image into view cleanly bounding via background properties mapped to the 21:9 container
        snapshotBox.style.backgroundImage = 'url("camera_feed_sample.png")';
        snapshotBox.style.backgroundSize = 'cover';
        snapshotBox.style.backgroundPosition = 'center';
        snapshotBox.textContent = ''; // Clear prior empty placeholder
        
        // Re-enable if still monitoring
        if (currentState === STATE_MONITORING) {
            snapshotBtn.disabled = false;
        }
    }, 1500);
});
