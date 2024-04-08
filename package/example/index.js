import { AppRegistry } from 'react-native'
import App from './src/App'
import { name as appName } from './app.json'

// Run filament tests
import { runTests } from 'react-native-filament-test'
runTests()

AppRegistry.registerComponent(appName, () => App)
