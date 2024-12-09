import React from 'react'
import { SafeAreaView, Text, View } from 'react-native'
import LinearGradient from 'react-native-linear-gradient'
import Menu from 'react-native-vector-icons/FontAwesome6';
import { useNavigation } from '@react-navigation/native';

export default function SettingsScreen() {
    const navigation = useNavigation(); // Hook to access navigation object

    return (
        <LinearGradient colors={['#000000', '#130F40']} style={{ flex: 1 }}>
            <SafeAreaView style={{ margin: 16 }}>
                <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'space-between',justifyContent:'flex-start' }}>
                    <Menu name="bars-staggered" size={25} color="white" onPress={() => navigation.openDrawer()} />
                    <Text style={{ color: 'white',paddingLeft:80,fontWeight:'bold',fontSize:28 }}>Setting</Text>
                </View>
            </SafeAreaView>
        </LinearGradient>
    )
}