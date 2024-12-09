import { View, Text, SafeAreaView, TouchableOpacity } from 'react-native'
import React, { useState, useEffect } from 'react'
import LinearGradient from 'react-native-linear-gradient';
import Arror from 'react-native-vector-icons/AntDesign';
import HomeScreen from './HomeScreen';
import { useNavigation } from '@react-navigation/native';
import Chart from '../Components/Chart';
import database from '@react-native-firebase/database';

export default function PowerScreen() {
    const navigation = useNavigation();
    const [currentPower, setcurrentPower] = useState('');

    useEffect(() => {
        // Function to read value from Firebase RTDB
        const fetchcurrentPower = async () => {
            try {
                const snapshot = await database().ref(`M3R/Power`).once('value');
                const value = snapshot.val();
                setcurrentPower(value);
            } catch (error) {
                console.error('Error fetching used today value:', error);
            }
        };

        // Call the function to fetch the value when the component mounts
        fetchcurrentPower();
    }, []);

    return (
        <LinearGradient colors={['#000000', '#130F40']} style={{ flex: 1 }}>
            <SafeAreaView style={{ margin: 16 }}>
                <View style={{ flexDirection: 'row', alignItems: 'center', justifyContent: 'flex-start' }}>
                    <TouchableOpacity style={{ backgroundColor: '#5D6D7E', borderRadius: 30, padding: 5 }}>
                        <Arror name="arrowleft" size={25} color="white" onPress={() => navigation.navigate(HomeScreen)} />
                    </TouchableOpacity>
                    <Text style={{ color: 'white', paddingLeft: 80, fontSize: 28, fontWeight: 'bold' }}>Current Details</Text>
                </View>
                <Text style={{ paddingVertical: 25, color: 'white', fontSize: 28, fontWeight: 'bold' }}>Information </Text>
                <Chart />
                <View style={{ flex: 1, flexDirection: 'row', justifyContent:'center',alignContent:'center', width: '100%', margin: 5 }}>
                    <LinearGradient colors={['#09203F', '#537895']} style={{ marginBottom: 10, padding: 10, width: "100%", height: 168, borderRadius: 10 }}>
                        <Text style={{fontSize:38,color:'white',fontWeight:'bold'}}>Used Today</Text>
                        <Text style={{ fontSize: 28, color: 'white', fontWeight: 'bold' }}>{currentPower} </Text>
                    </LinearGradient>
                </View>
            </SafeAreaView>
        </LinearGradient>
    )
}