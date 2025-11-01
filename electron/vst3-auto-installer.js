/**
 * ScaleChord VST3 Plugin - Auto Installer Module
 * Automatically installs the VST3 plugin to system locations on app startup
 */

const fs = require('fs');
const path = require('path');
const os = require('os');

class VST3AutoInstaller {
  constructor() {
    this.platform = process.platform;
    this.pluginSourcePath = null;
    this.pluginDestPaths = [];
    this.installLog = [];
  }

  /**
   * Initialize the installer with app paths
   * @param {string} appPath - Path to the app directory
   */
  initialize(appPath) {
    this.pluginSourcePath = path.join(appPath, 'vst3_plugin', 'ScaleChord.vst3');
    this.pluginDestPaths = this.getSystemVST3Paths();
    this.log(`VST3 Auto Installer initialized`);
    this.log(`Platform: ${this.platform}`);
    this.log(`Plugin source: ${this.pluginSourcePath}`);
    this.log(`Target paths: ${this.pluginDestPaths.join(', ')}`);
  }

  /**
   * Get all system VST3 plugin paths for the current platform
   * @returns {Array} Array of plugin directory paths
   */
  getSystemVST3Paths() {
    const paths = [];

    if (this.platform === 'darwin') {
      // macOS paths
      paths.push(path.join(os.homedir(), 'Library', 'Audio', 'Plug-Ins', 'VST3'));
      paths.push('/Library/Audio/Plug-Ins/VST3');
    } else if (this.platform === 'win32') {
      // Windows paths
      paths.push(path.join(
        os.homedir(),
        'AppData', 'Local', 'Programs', 'Common Files', 'VST3'
      ));
      // Also try standard Windows location
      paths.push('C:\\Program Files\\Common Files\\VST3');
      paths.push('C:\\Program Files (x86)\\Common Files\\VST3');
    } else if (this.platform === 'linux') {
      // Linux paths
      paths.push(path.join(os.homedir(), '.vst3'));
      paths.push('/usr/lib/vst3');
      paths.push('/usr/local/lib/vst3');
    }

    return paths;
  }

  /**
   * Check if plugin is already installed
   * @returns {boolean} True if plugin found in any system location
   */
  isPluginInstalled() {
    for (const destPath of this.pluginDestPaths) {
      const pluginPath = path.join(destPath, 'ScaleChord.vst3');
      if (fs.existsSync(pluginPath)) {
        this.log(`✓ Plugin found at: ${pluginPath}`);
        return true;
      }
    }
    this.log(`✗ Plugin not found in any system location`);
    return false;
  }

  /**
   * Automatically install plugin to first available location
   * @returns {Object} Installation result {success, path, error}
   */
  autoInstall() {
    try {
      // Check if source exists
      if (!fs.existsSync(this.pluginSourcePath)) {
        const error = `Plugin source not found: ${this.pluginSourcePath}`;
        this.log(`✗ ${error}`);
        return { success: false, error };
      }

      this.log(`✓ Plugin source found`);

      // Try to install to each path
      for (const destBasePath of this.pluginDestPaths) {
        try {
          const destPath = path.join(destBasePath, 'ScaleChord.vst3');

          // Create directory if needed
          if (!fs.existsSync(destBasePath)) {
            this.log(`Creating directory: ${destBasePath}`);
            fs.mkdirSync(destBasePath, { recursive: true });
          }

          // Copy plugin
          this.log(`Copying plugin to: ${destPath}`);
          this.copyRecursive(this.pluginSourcePath, destPath);

          this.log(`✓ Plugin installed successfully to: ${destPath}`);
          return { 
            success: true, 
            path: destPath,
            message: `Plugin installed to ${destPath}`
          };
        } catch (error) {
          this.log(`⚠ Failed to install to ${destBasePath}: ${error.message}`);
          continue;
        }
      }

      const error = `Could not install to any location. Check permissions.`;
      this.log(`✗ ${error}`);
      return { success: false, error };

    } catch (error) {
      this.log(`✗ Installation error: ${error.message}`);
      return { success: false, error: error.message };
    }
  }

  /**
   * Recursively copy folder
   * @param {string} src - Source path
   * @param {string} dest - Destination path
   */
  copyRecursive(src, dest) {
    if (!fs.existsSync(dest)) {
      fs.mkdirSync(dest, { recursive: true });
    }

    const files = fs.readdirSync(src);
    for (const file of files) {
      const srcPath = path.join(src, file);
      const destPath = path.join(dest, file);

      const stat = fs.lstatSync(srcPath);
      if (stat.isDirectory()) {
        this.copyRecursive(srcPath, destPath);
      } else {
        fs.copyFileSync(srcPath, destPath);
      }
    }
  }

  /**
   * Add message to installation log
   * @param {string} message - Log message
   */
  log(message) {
    const timestamp = new Date().toLocaleTimeString();
    const logMessage = `[${timestamp}] ${message}`;
    this.installLog.push(logMessage);
    console.log(logMessage);
  }

  /**
   * Get installation log
   * @returns {Array} Installation log messages
   */
  getLog() {
    return this.installLog;
  }

  /**
   * Get installation log as formatted string
   * @returns {string} Formatted log
   */
  getLogString() {
    return this.installLog.join('\n');
  }
}

module.exports = VST3AutoInstaller;
