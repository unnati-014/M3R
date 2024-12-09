import React from 'react';
import { SafeAreaView, StatusBar } from 'react-native';
import  AppNavigation  from './src/Navigation/AppNavigation';

const App = () => {
  return (
    <>
      <StatusBar barStyle="dark-content" />
      <SafeAreaView style={{ flex: 1 }}>
        <AppNavigation />
      </SafeAreaView>
    </>
  );
};

export default App;
