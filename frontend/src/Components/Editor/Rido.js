import React, {useEffect, useReducer} from 'react';
import {useParams} from "react-router-dom";

import './Editor.css';

export const DatasetDispatch = React.createContext(null);

function datasetReducer(dataset, action) {
  console.log(action.dataset)
  switch (action.type) {
    case 'setDataset':
      return action.dataset;
    default:
      throw new Error();
  }
}

function Rido(props) {
  useEffect(() => {
    document.title = params.fileName
  }, [params.fileName]);

  // console.log(params.fileName)

  let params = useParams();

  const [dataset, datasetDispatch] = useReducer(datasetReducer, "");

  function fetchFile() {
    fetch(props.url.backendc + '/editor/Gentilly/Ind1', {
      method: 'POST',
      // mode: 'no-cors', // ***************** FOR TEST ONLY *******************
      headers: {
        // 'Access-Control-Allow-Headers': '*',
        // 'Access-Control-Allow-Origin': "http://localhost:9000",
        // "Access-Control-Allow-Headers": "Content-Type",
        // 'Access-Control-Allow-Origin': '*',
        'Content-Type': 'application/json'
      },
      body: JSON.stringify({
        user: params.user,
        fileName: params.fileName
      })
    })
    .then(response => {
      // console.log(response);
      return response.json();
    })
    .then(data => {
      // console.log("data:", data)
      datasetDispatch({'type': 'setDataset', 'dataset': data})
    })
    .catch(error => {
      console.error('Error:', error);
    }); 
  }
  
  useEffect(fetchFile, [params, props])
  

  return <div>{"dataset"}</div>
  
}

export default Rido;