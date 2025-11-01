// ============================================
// SCALECHORD ELECTRON APP - RENDERER PROCESS
// ============================================

// Initialize app on load
document.addEventListener('DOMContentLoaded', initializeApp);

// ============================================
// INITIALIZATION
// ============================================

async function initializeApp() {
  console.log('Initializing ScaleChord Electron App...');
  
  try {
    // Load plugin info
    await loadPluginInfo();
    
    // Load VST3 paths
    await loadVST3Paths();
    
    // Check plugin installation status
    await checkPluginStatus();
    
    console.log('App initialized successfully');
  } catch (error) {
    console.error('Error initializing app:', error);
    showNotification('Error initializing app', 'error');
  }
}

// ============================================
// PLUGIN INFORMATION
// ============================================

async function loadPluginInfo() {
  try {
    const info = await window.electronAPI.getPluginInfo();
    
    document.getElementById('pluginName').textContent = info.name;
    document.getElementById('pluginVersion').textContent = info.version;
    
    console.log('Plugin info loaded:', info);
  } catch (error) {
    console.error('Error loading plugin info:', error);
  }
}

// ============================================
// VST3 PATHS
// ============================================

async function loadVST3Paths() {
  try {
    const paths = await window.electronAPI.getVST3Paths();
    
    document.getElementById('userVstPath').textContent = paths.user;
    document.getElementById('systemVstPath').textContent = paths.system;
    
    console.log('VST3 paths loaded:', paths);
  } catch (error) {
    console.error('Error loading VST3 paths:', error);
  }
}

// ============================================
// PLUGIN STATUS
// ============================================

async function checkPluginStatus() {
  try {
    const status = await window.electronAPI.getPluginStatus();
    
    const statusIndicator = document.getElementById('statusIndicator');
    const statusText = statusIndicator.querySelector('.status-text');
    const statusDot = statusIndicator.querySelector('.status-dot');
    const installStatus = document.getElementById('installStatus');
    
    if (status.installed) {
      statusText.textContent = 'Installed';
      statusDot.classList.remove('disconnected');
      installStatus.textContent = 'Installed ✓';
      installStatus.classList.remove('not-installed');
      installStatus.style.color = '#00dd00';
    } else {
      statusText.textContent = 'Not Installed';
      statusDot.classList.add('disconnected');
      installStatus.textContent = 'Not Installed';
      installStatus.classList.add('not-installed');
      installStatus.style.color = '#ffaa00';
    }
    
    console.log('Plugin status:', status);
  } catch (error) {
    console.error('Error checking plugin status:', error);
  }
}

// ============================================
// INSTALLATION
// ============================================

async function installPlugin() {
  const btn = document.getElementById('autoInstallBtn');
  const note = document.getElementById('autoInstallNote');
  
  try {
    btn.disabled = true;
    btn.textContent = '⏳ Installing...';
    
    const result = await window.electronAPI.installPlugin();
    
    if (result.success) {
      showNotification('✓ Plugin installed successfully!', 'success');
      note.textContent = result.message;
      note.style.color = '#00dd00';
      
      // Refresh status
      setTimeout(() => checkPluginStatus(), 1000);
    } else {
      showNotification('✗ Installation failed: ' + result.error, 'error');
      note.textContent = 'Error: ' + result.error;
      note.style.color = '#ff3333';
    }
  } catch (error) {
    console.error('Error installing plugin:', error);
    showNotification('Installation error: ' + error.message, 'error');
    note.textContent = 'Error: ' + error.message;
    note.style.color = '#ff3333';
  } finally {
    btn.disabled = false;
    btn.textContent = '🚀 Install Plugin';
  }
}

async function browseForInstallation() {
  try {
    const result = await window.electronAPI.openFileDialog({
      properties: ['openDirectory'],
      title: 'Select VST3 Installation Directory'
    });
    
    if (!result.canceled && result.filePaths.length > 0) {
      console.log('Selected directory:', result.filePaths[0]);
      showNotification('Directory selected: ' + result.filePaths[0], 'success');
    }
  } catch (error) {
    console.error('Error browsing directory:', error);
    showNotification('Error opening file dialog', 'error');
  }
}

// ============================================
// UTILITY FUNCTIONS
// ============================================

function copyToClipboard(selector) {
  const element = document.querySelector(selector);
  if (!element) return;
  
  const text = element.textContent;
  navigator.clipboard.writeText(text).then(() => {
    showNotification('✓ Copied to clipboard', 'success');
  }).catch(err => {
    console.error('Error copying to clipboard:', err);
    showNotification('Error copying to clipboard', 'error');
  });
}

function showNotification(message, type = 'info') {
  const toast = document.getElementById('notificationToast');
  const toastMessage = document.getElementById('toastMessage');
  
  toastMessage.textContent = message;
  toast.className = `toast show ${type}`;
  
  // Auto-hide after 4 seconds
  setTimeout(() => {
    toast.classList.remove('show');
  }, 4000);
}

// ============================================
// NAVIGATION
// ============================================

function openDocumentation() {
  shell.openExternal('https://scalechord.example.com/docs');
  showNotification('Opening documentation...');
}

function reportIssue() {
  shell.openExternal('https://github.com/scalechord/plugin/issues');
  showNotification('Opening GitHub issues...');
}

function joinCommunity() {
  shell.openExternal('https://discord.gg/scalechord');
  showNotification('Opening community Discord...');
}

function watchTutorials() {
  shell.openExternal('https://www.youtube.com/playlist?list=PLxxx');
  showNotification('Opening tutorials...');
}

// ============================================
// ERROR HANDLING
// ============================================

window.addEventListener('error', (event) => {
  console.error('Global error:', event.error);
  showNotification('Error: ' + event.error.message, 'error');
});

window.addEventListener('unhandledrejection', (event) => {
  console.error('Unhandled promise rejection:', event.reason);
  showNotification('Error: ' + event.reason, 'error');
});

// ============================================
// AUTO-REFRESH
// ============================================

// Check plugin status every 10 seconds
setInterval(checkPluginStatus, 10000);

// ============================================
// KEYBOARD SHORTCUTS
// ============================================

document.addEventListener('keydown', (event) => {
  // Ctrl/Cmd + R: Refresh plugin status
  if ((event.ctrlKey || event.metaKey) && event.key === 'r') {
    event.preventDefault();
    checkPluginStatus();
    showNotification('Refreshed plugin status', 'info');
  }
  
  // Ctrl/Cmd + I: Install plugin
  if ((event.ctrlKey || event.metaKey) && event.key === 'i') {
    event.preventDefault();
    installPlugin();
  }
});

// ============================================
// FEATURE DETECTION
// ============================================

function detectPlatform() {
  const userAgent = navigator.userAgent;
  
  if (userAgent.includes('Win')) return 'windows';
  if (userAgent.includes('Mac')) return 'macos';
  if (userAgent.includes('Linux')) return 'linux';
  
  return 'unknown';
}

// Add platform class to body
document.body.classList.add(`platform-${detectPlatform()}`);

// ============================================
// ACCESSIBILITY
// ============================================

// Focus management
document.addEventListener('keydown', (event) => {
  if (event.key === 'Tab') {
    document.body.classList.add('keyboard-nav');
  }
});

document.addEventListener('mousedown', () => {
  document.body.classList.remove('keyboard-nav');
});

// High contrast mode detection
if (window.matchMedia('(prefers-contrast: more)').matches) {
  document.body.classList.add('high-contrast');
}

// Reduced motion detection
if (window.matchMedia('(prefers-reduced-motion: reduce)').matches) {
  document.documentElement.style.setProperty('--transition', 'none');
}

console.log('ScaleChord Electron App loaded successfully');
