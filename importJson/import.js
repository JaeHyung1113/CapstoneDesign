const { initializeFirebaseApp, restore } = require('firestore-export-import')
const firebaseConfig = require('./config.js');
const serviceAccount = require('./serviceAccount.json');



// JSON To Firestore

const jsonToFirestore = async () => {
try {
console.log('Initialzing Firebase');
const  appName  =  '[DEFAULT]'
initializeFirebaseApp(serviceAccount, firebaseConfig.databaseURL, appName)
console.log('Firebase Initialized');

restore('./fallWarm.json');
restore('./springWarm.json');
restore('./summerCool.json');
restore('./winterCool.json');

console.log('Upload Success');

}

catch (error) {
console.log(error);

}

};

jsonToFirestore();