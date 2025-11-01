const { contextBridge, ipcRenderer } = require('electron');

// Expose safe IPC methods to renderer process
contextBridge.exposeInMainWorld('electronAPI', {
  // Plugin management
  getPluginStatus: () => ipcRenderer.invoke('get-plugin-status'),
  installPlugin: () => ipcRenderer.invoke('install-plugin'),
  getVST3Paths: () => ipcRenderer.invoke('get-vst3-paths'),
  getPluginInfo: () => ipcRenderer.invoke('get-plugin-info'),
  openFileDialog: (options) => ipcRenderer.invoke('open-file-dialog', options),

  // File operations
  readFile: (filePath) => ipcRenderer.invoke('read-file', filePath),
  writeFile: (filePath, content) => ipcRenderer.invoke('write-file', filePath, content),
  
  // System info
  getPlatform: () => ipcRenderer.invoke('get-platform'),
  
  // Event listeners
  onUpdate: (callback) => ipcRenderer.on('app-update', callback),
  onError: (callback) => ipcRenderer.on('app-error', callback)
});

// Expose console methods for development
if (process.env.NODE_ENV === 'development') {
  contextBridge.exposeInMainWorld('console', console);
}
