'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');

const file_path = './jsonexamples/small/themoviedb.json';

const file = fs.readFileSync(file_path, 'utf-8');
var JSONObj = JSON.parse(file);

// Affiche le contenu de l'objet
//console.log(JSONObj);

// Affiche la taille de l'objet (tableau seulement)
//console.log(JSONObj.length);

// Affiche les cles de l'objet
//console.log(Object.keys(JSONObj));

// Parcours de l'objet (tableau seulement)
try {
    for (let item of JSONObj) {
        console.log(item);
    }
} catch(error) {
    console.error(error);
}

// Parcours de l'objet et affichage de propriétés imbriquées spécifiques (tableau seulement)
/*try {
    for (let item of JSONObj) {
        console.log('actor : ');
        console.log(item.actor);
        console.log();

        console.log('repo : ');
        console.log(item.repo);
        console.log();

        console.log('payload.forkee.owner : ');
        if(item.payload && item.payload.forkee) {
            console.log(item.payload.forkee.owner);
        }
    }
} catch(error) {
    console.error(error);
}*/