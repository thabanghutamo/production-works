const { app, BrowserWindow, Menu, ipcMain, dialog } = require('electron');
const path = require('path');
const fs = require('fs');
const os = require('os');
const VST3AutoInstaller = require('./vst3-auto-installer');

let mainWindow;
let pluginServerProcess;
let vst3Installer;

// Create application window
function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1200,
    height: 800,
    minWidth: 800,
    minHeight: 600,
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      contextIsolation: true,
      enableRemoteModule: false,
      sandbox: true
    },
    icon: path.join(__dirname, 'assets/icon.png')
  });

  mainWindow.loadFile('index.html');

  // Open DevTools in development mode
  if (process.env.NODE_ENV === 'development') {
    mainWindow.webContents.openDevTools();
  }

  mainWindow.on('closed', function () {
    mainWindow = null;
  });
}

// App event handlers
app.on('ready', () => {
  // Initialize VST3 auto-installer
  vst3Installer = new VST3AutoInstaller();
  vst3Installer.initialize(__dirname);
  
  // Auto-install plugin if not already installed
  if (!vst3Installer.isPluginInstalled()) {
    console.log('Plugin not installed. Starting auto-installation...');
    const result = vst3Installer.autoInstall();
    console.log('Auto-installation result:', result);
  } else {
    console.log('Plugin already installed, skipping auto-installation');
  }
  
  createWindow();
});

app.on('window-all-closed', function () {
  if (process.platform !== 'darwin') {
    app.quit();
  }
});

app.on('activate', function () {
  if (mainWindow === null) {
    createWindow();
  }
});

// IPC handlers
ipcMain.handle('get-plugin-status', async () => {
  return {
    name: 'ScaleChord VST3 Plugin',
    version: '1.0.0',
    status: 'ready',
    installed: isPluginInstalled()
  };
});

ipcMain.handle('install-plugin', async () => {
  try {
    const sourcePath = path.join(__dirname, 'vst3_plugin/ScaleChord.vst3');
    const destPath = getVST3Path();

    if (!fs.existsSync(sourcePath)) {
      return {
        success: false,
        error: 'Plugin binary not found'
      };
    }

    // Create directory if it doesn't exist
    if (!fs.existsSync(destPath)) {
      fs.mkdirSync(destPath, { recursive: true });
    }

    // Copy plugin
    copyFolderRecursive(sourcePath, destPath);

    return {
      success: true,
      message: `Plugin installed to ${destPath}`
    };
  } catch (error) {
    return {
      success: false,
      error: error.message
    };
  }
});

ipcMain.handle('get-vst3-paths', async () => {
  const paths = {};
  const platform = process.platform;

  if (platform === 'darwin') {
    paths.user = `${os.homedir()}/Library/Audio/Plug-Ins/VST3`;
    paths.system = '/Library/Audio/Plug-Ins/VST3';
  } else if (platform === 'win32') {
    paths.user = `${os.homedir()}\\AppData\\Local\\Programs\\Common Files\\VST3`;
    paths.system = 'C:\\Program Files\\Common Files\\VST3';
  } else {
    paths.user = `${os.homedir()}/.vst3`;
    paths.system = '/usr/local/lib/vst3';
  }

  return paths;
});

ipcMain.handle('open-file-dialog', async (event, options) => {
  return await dialog.showOpenDialog(mainWindow, options);
});

ipcMain.handle('get-installation-log', async () => {
  if (vst3Installer) {
    return {
      success: true,
      log: vst3Installer.getLog()
    };
  }
  return {
    success: false,
    error: 'Installer not initialized'
  };
});

ipcMain.handle('get-plugin-info', async () => {
  return {
    name: 'ScaleChord VST3 Plugin',
    version: '1.0.0',
    description: 'Intelligent MIDI voicing and chord generation for music production',
    features: [
      'Scale-aware chord voicing',
      'Multiple voice leading algorithms',
      'Real-time MIDI processing',
      '13+ built-in scales',
      'Jazz reharmonization',
      'Full DAW automation'
    ],
    requirements: {
      cpu: '2 cores minimum',
      ram: '512 MB minimum',
      storage: '50 MB'
    }
  };
});

// Helper functions
function getVST3Path() {
  const platform = process.platform;
  let basePath;

  if (platform === 'darwin') {
    basePath = `${os.homedir()}/Library/Audio/Plug-Ins/VST3`;
  } else if (platform === 'win32') {
    basePath = `${os.homedir()}\\AppData\\Local\\Programs\\Common Files\\VST3`;
  } else {
    basePath = `${os.homedir()}/.vst3`;
  }

  return path.join(basePath, 'ScaleChord.vst3');
}

function isPluginInstalled() {
  try {
    const pluginPath = getVST3Path();
    return fs.existsSync(pluginPath);
  } catch (error) {
    return false;
  }
}

function copyFolderRecursive(src, dest) {
  if (!fs.existsSync(dest)) {
    fs.mkdirSync(dest, { recursive: true });
  }

  fs.readdirSync(src).forEach(file => {
    const srcPath = path.join(src, file);
    const destPath = path.join(dest, file);

    if (fs.lstatSync(srcPath).isDirectory()) {
      copyFolderRecursive(srcPath, destPath);
    } else {
      fs.copyFileSync(srcPath, destPath);
    }
  });
}

// Create application menu
function createMenu() {
  const template = [
    {
      label: 'File',
      submenu: [
        {
          label: 'Exit',
          accelerator: 'CmdOrCtrl+Q',
          click: () => {
            app.quit();
          }
        }
      ]
    },
    {
      label: 'Help',
      submenu: [
        {
          label: 'About ScaleChord',
          click: () => {
            dialog.showMessageBox(mainWindow, {
              type: 'info',
              title: 'About ScaleChord VST3 Plugin',
              message: 'ScaleChord VST3 Plugin v1.0.0',
              detail: 'Intelligent MIDI voicing and chord generation for music production'
            });
          }
        },
        {
          label: 'Documentation',
          click: () => {
            require('electron').shell.openExternal('https://scalechord.example.com/docs');
          }
        }
      ]
    }
  ];

  if (process.platform === 'darwin') {
    template.unshift({
      label: app.name,
      submenu: [
        {
          label: 'About ScaleChord',
          role: 'about'
        },
        { type: 'separator' },
        {
          label: 'Preferences',
          accelerator: 'Cmd+,'
        },
        { type: 'separator' },
        {
          label: 'Quit',
          accelerator: 'Cmd+Q',
          click: () => {
            app.quit();
          }
        }
      ]
    });
  }

  Menu.setApplicationMenu(Menu.buildFromTemplate(template));
}

app.on('ready', createMenu);
