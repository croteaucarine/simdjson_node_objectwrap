'use strict';

const fs = require('fs');
const { simdjson } = require('bindings')('addon');

const file_path = './jsonexamples/github_events.json';
const file = fs.readFileSync(file_path, 'utf-8');
const simdjsonOBJ = simdjson.lazyParse1Depth(file);

// Affiche le contenu de l'objet
console.log("objet");
console.log(simdjsonOBJ);

// Affiche la taille de l'objet (tableau seulement)
console.log("length");
console.log(simdjsonOBJ.length);

// Affiche les cles de l'objet
console.log("keys");
console.log(Object.keys(simdjsonOBJ));

// Parcours de l'objet (tableau seulement)
console.log("Parcours");
try {
    for (let item of simdjsonOBJ) {
        console.log(item);
    }
} catch(error) {
    console.error(error);
}

// Parcours de l'objet et affichage de propriétés imbriquées spécifiques (tableau seulement)
try {
    for (let item of simdjsonOBJ) {
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
}