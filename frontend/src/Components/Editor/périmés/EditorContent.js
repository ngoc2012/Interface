import React from 'react';
import {Route, Redirect} from "react-router-dom";
import GetJson from './Form/GetJson';
import Csv from './Csv/Csv';


function getFileExtension(filename) {
  return filename.split('.').pop();
}

function EditorContent(props) {

  const extension = getFileExtension(props.file)

  switch (extension.toLowerCase()) {
    case "csv":
      return (<Csv></Csv>)
      // return (<Route><Redirect to={"/editor/" + extension}/>}</Route>)
    case "sec":
      return (<Route><Redirect to="/editor/wallSection"/></Route>)
    case "mat":
    case "bpi":
      return (<GetJson url="/editor/RCmaterials" extension={extension.toLowerCase()}></GetJson>)
    case "lst":
      return (<GetJson url="/editor/RIDOLST" extension={extension.toLowerCase()}></GetJson>)
    default:
      break;
  }
  
  return (<div>Le type de fichier {extension} n'est pas supporté</div>)
  
}

export default EditorContent;