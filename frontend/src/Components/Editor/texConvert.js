import React from 'react';

import Tex2SVG from "react-hook-mathjax";

import data2Groupes from "./groupes";
import {getAttrKeyLevel} from "./attr";
import {InputData} from "./dataElement";

export default function texConvert(dataset) {
  let data = data2array(dataset)
  return arrayData2DOM(data, dataset)
}

// $2020$ => 2020
function replaceNumbers(text) {
  function replacerNumber(match, p1, offset, string) {return p1;}
  const regex = /\$(-?\d+(\.\d*)?((e|E)(\+|-)\d+)?)\$/g;
  return text.replace(regex, replacerNumber);
}

function getInputOutputTermes(text, dataset) {
  // console.log(dataset.output)
  let parts = text.split(/\[{(\S+)}\]/g)
  
  for (let i = 1; i < parts.length; i+= 2) {
    if (getAttrKeyLevel(dataset.input, parts[i]) === null) {
      parts[i] = getAttrKeyLevel(dataset.output, parts[i])
    } else {
      parts[i] = <InputData key={i} attr={parts[i]} value={getAttrKeyLevel(dataset.input, parts[i])} />
    }
  }
  // console.log(parts)
  return parts
}

// Get latex equation
function replaceEquations(text, key) {
  if (((typeof text) !== "string") | (text === "")) {
    return text
  }
  let re = /\s*\$\$(.+)\$\$\s*/g
  let fullEquation = re.exec(text)
  /*[
    '   $$ f_{ck} = \frac{f_{cd}}{1,5} $$   ',
    ' f_{ck} = \frac{f_{cd}}{1,5} ',
    index: 0,
    input: '   $$ f_{ck} = \frac{f_{cd}}{1,5} $$   ',
    groups: undefined
  ]*/
  
  if (fullEquation !== null) {
    return <Tex2SVG key={key} latex={fullEquation[1]} />
  } else {
    let parts = text.split(/\$(.+)\$/g)
    for (let i = 1; i < parts.length; i+= 2) {
      parts[i] = <Tex2SVG key={i} display="inline" latex={parts[i]} />
    }
    return parts
  }
}

function updateText(text, dataset) {
  text = replaceNumbers(text);
  text = getInputOutputTermes(text, dataset);
  text.forEach((t, i) => {text[i] = replaceEquations(t, i)})
  // text = replaceEquations(text, 1)
  return text;
}

function arrayData2DOM(arrayData, dataset) {
  return <div>
    {arrayData.map((dom, i) => {
      switch (dom.type) {
        case 'line':
          return <div key={i}>{updateText(dom.value, dataset)}</div>;
        case 'section':
          return <h3 key={i}>{updateText(dom.value, dataset)}</h3>;
        case 'subsection':
          return <h4 key={i}>{updateText(dom.value, dataset)}</h4>;
        case 'itemize':
          return <ul key={i}>
            {dom.value.map((v,ii) => <li key={ii}>{updateText(v, dataset).replace("\\item ", "")}</li>)}
          </ul>
        case 'br':
          return <br/>
          // break;
        case 'a':
          return <a href={dom.href} key={i}>{dom.value}</a>;
        default:
          return "";
      }
    })}
  </div>
}

// \underline{Nota :} Les pieux sont en compression centrée
function findFunctions(line) {
  const functionsName = ['section', 'subsection']

  let functionValue = null
  /* Condition: function must be in a line */
  functionsName.forEach(f => {
    let position = line.indexOf('\\' + f + '{')
    if (position !== -1) {
      // console.log(line, position)
      functionValue = {"type": f, "value": line.substring(position + f.length + 2, line.length - 1)}
    }
  })
  return functionValue
}

function groupe2array(g) {
  let arrayData = []
  g.value.forEach(line => {
    let functionValue = findFunctions(line)
    if (functionValue === null) {
      arrayData.push({"type": "line", "value": line})
    } else {
      arrayData.push(functionValue)
    }
  });

  return arrayData;
}

function data2array(dataset) {
  let groupes = data2Groupes(dataset.tex)
  // console.log(groupes)
  let arrayData = []
  groupes.forEach(g => {
    if (g.type === "line") {
      arrayData = arrayData.concat(groupe2array(g))
    } else if (g.type === "itemize") {
      arrayData = arrayData.concat(g)
    }
  })
  return arrayData
}
