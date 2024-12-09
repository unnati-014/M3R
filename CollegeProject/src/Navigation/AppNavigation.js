import * as React from 'react';
import { StyleSheet, View } from 'react-native'
import { createDrawerNavigator } from '@react-navigation/drawer';
import { NavigationContainer } from '@react-navigation/native';
import { createNativeStackNavigator } from '@react-navigation/native-stack';
import HomeScreen from '../Screens/HomeScreen';
import ProfileScreen from '../Screens/ProfileScreen';
import SettingsScreen from '../Screens/SettingScreen';
import VoltageScreen from '../Screens/VoltageScreen';
import PowerScreen from '../Screens/PowerScreen';

const Drawer = createDrawerNavigator();
const Stack = createNativeStackNavigator();

export default function AppNavigation() {
    return (
        <NavigationContainer>
            <Drawer.Navigator initialRouteName="Home" screenOptions={{headerShown: false,
                drawerStyle:styles.Drawer,
                drawerLabelStyle:styles.option,
                }}>
                <Drawer.Screen name="Home" component={HomeStackScreen} />
                <Drawer.Screen name="Setting" component={SettingsScreen} />
                <Drawer.Screen name="Profile" component={ProfileScreen} />
            </Drawer.Navigator>
        </NavigationContainer>
    );
}

function HomeStackScreen() {
    return (
        <Stack.Navigator screenOptions={{ headerShown:false }}>
            <Stack.Screen name="HomeScreen" component={HomeScreen} />
            <Stack.Screen name="PowerScreen" component={PowerScreen} /> 
            <Stack.Screen name="VoltageScreen" component={VoltageScreen} /> 
        </Stack.Navigator>
    );
}


const styles = StyleSheet.create({
    Drawer:{
        backgroundColor:'#36454F',
    },
    option:{
        color:'white'
    }
})
