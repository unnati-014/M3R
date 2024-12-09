import { React,  } from 'react';
import { View, SafeAreaView, Text, FlatList, Image, TouchableOpacity } from 'react-native';
import LinearGradient from 'react-native-linear-gradient';
import { useNavigation } from '@react-navigation/native';

//icons
import Menu from 'react-native-vector-icons/FontAwesome6';
import User from 'react-native-vector-icons/Feather';
import Power from 'react-native-vector-icons/MaterialIcons';
import Arror from 'react-native-vector-icons/AntDesign';

//screens
import VoltageScreen from './VoltageScreen';
import PowerScreen from './PowerScreen';
import List from '../Components/List';

export default function HomeScreen() {
    const navigation = useNavigation(); // Hook to access navigation object

    return (
        <LinearGradient colors={['#000000', '#130F40']} style={{ flex: 1 }}>
            <SafeAreaView style={{ margin: 16 }}>
                <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'space-between' }}>
                    <Menu name="bars-staggered" size={25} color="white" onPress={() => navigation.openDrawer()} />
                    <User name="user" size={30} color="white" />
                </View>
                <Text style={{ marginTop: 15, marginBottom: 15, color: 'white', fontSize: 28, fontWeight: 'bold' }}>Hello User👋</Text>
                <View style={{ borderRadius: 14, backgroundColor: '#36454F', marginTop: 10, flexDirection: 'row', justifyContent: 'space-around', alignItems: 'center', height: 65 }}>
                    <LinearGradient colors={['#A4BFEF', '#6A93CB']} style={{ borderRadius: 30, padding: 10 }}>
                        <Power name="electric-bolt" size={30} color="white" />
                    </LinearGradient>
                    <Text style={{ color: 'white', fontWeight: 'bold', fontSize: 20 }}>Consumed Power</Text>
                    <TouchableOpacity style={{ backgroundColor: '#5D6D7E', borderRadius: 30, padding: 10 }}>
                        <Arror name="arrowright" size={30} color="white" onPress={() => navigation.navigate(PowerScreen)} />
                    </TouchableOpacity>
                </View>
                <View style={{ borderRadius: 14, backgroundColor: '#36454F', marginTop: 10, marginBottom: 15, flexDirection: 'row', justifyContent: 'space-around', alignItems: 'center', height: 65 }}>
                    <LinearGradient colors={['#A4BFEF', '#6A93CB']} style={{ borderRadius: 30, padding: 10 }}>
                        <Image source={require('../Assets/Images/voltage.png')} style={{ width: 30, height: 30 }} />
                    </LinearGradient>
                    <Text style={{ color: 'white', fontWeight: 'bold', fontSize: 20 }}>Voltage</Text>
                    <TouchableOpacity style={{ backgroundColor: '#5D6D7E', borderRadius: 30, padding: 10 }}>
                        <Arror name="arrowright" size={30} color="white" onPress={() => navigation.navigate(VoltageScreen)} />
                    </TouchableOpacity>
                </View>
                <Text style={{ marginTop: 10, color: 'white', fontWeight: 'bold', fontSize: 20 }}>Linked to you</Text>
                <List />
            </SafeAreaView>
        </LinearGradient>
    );
}
