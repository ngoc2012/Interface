import React, {useEffect, useReducer} from 'react';
import {useParams, useLocation} from "react-router-dom";
import {MathJaxProvider} from "react-hook-mathjax";

import './Editor.css';
import texConvert from './texConvert';
import {setAttrKeyLevel} from "./attr";
import ChartTopDown from "../Graph/ChartTopDown";

export const DatasetDispatch = React.createContext(null);

function datasetReducer(dataset, action) {
  switch (action.type) {
    case 'setDataset':
      return action.dataset;
    case 'updateInput':
      setAttrKeyLevel(dataset.input, action.attr, action.value)
      // console.log(getAttrKeyLevel(dataset.input, action.attr));
      return dataset;
    default:
      throw new Error();
  }
}

function Editor(props) {

  // console.log(props);
  let params = useParams();
  let location = useLocation();

  useEffect(() => {
    document.title = location.pathname.replace("/editor/ngoc2012/", "")
  }, [location.pathname]);

  const [dataset, datasetDispatch] = useReducer(datasetReducer, {'type': "", 'status': ""});

  // console.log(params);
  // console.log(location.pathname);

  function fetchFile() {
    fetch(props.url.backend + location.pathname, {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: {}
    })
    .then(response => response.json())
    .then(data => datasetDispatch({'type': 'setDataset', 'dataset': data}))
    .catch(error => {console.error('Error:', error);}); 
  }
  
  useEffect(fetchFile, [params, props])
  
  // http://docs.mathjax.org/en/latest/options/output/svg.html
  const mathJaxOptions = {
    svg: {
      scale: 1,                      // global scaling factor for all expressions
      minScale: .5,                  // smallest scaling factor to use
      mtextInheritFont: false,       // true to make mtext elements use surrounding font
      merrorInheritFont: true,       // true to make merror text use surrounding font
      mathmlSpacing: false,          // true for MathML spacing rules, false for TeX rules
      skipAttributes: {},            // RFDa and other attributes NOT to copy to the output
      exFactor: .5,                  // default size of ex in em units
      displayAlign: 'center',        // default for indentalign when set to 'auto'
      displayIndent: '0',            // default for indentshift when set to 'auto'
      fontCache: 'local',            // or 'global' or 'none'
      localID: null,                 // ID to use for local font cache (for single equation processing)
      internalSpeechTitles: true,    // insert <title> tags with speech content
      titleID: 0                     // initial id number to use for aria-labeledby titles
    }
  }

  return <DatasetDispatch.Provider value={datasetDispatch}>
    <MathJaxProvider options={mathJaxOptions}>
      <div className="editor">
        {dataset.type === "error" && (<div>{dataset.status}</div>)}
        {dataset.type === "json" && (<div>{texConvert(dataset)}</div>)}
        {dataset.type === "graphRidoBA" && (<ChartTopDown {...props} {...dataset}></ChartTopDown>)}
      </div>
    </MathJaxProvider>
  </DatasetDispatch.Provider>
  
}

export default Editor;