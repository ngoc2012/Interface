import React, {useReducer, useEffect, useState} from 'react'
import {useHistory, useLocation} from "react-router-dom";

import Folder from './Folder';
import File from './File';

import {browserContext} from './browserContext';
import browserReducer from './browserReducer';
import key2list from './key2list';
import './Browser.css';


function Browser(props) {
  
  let history = useHistory();
  let location = useLocation();

  if (props.user === null) {
    props.setStatus(">> Merci de s'identifier!!!")
    let { from } = location.state || { from: { pathname: "/login" } };
    history.replace(from);
  }

  // const [files, setFiles] = useState([])
  const browserValue = useReducer(browserReducer, {
    status: "...",
    activeFile: null,
    fileList: []
  });

  const [status, setStatus] = useState("...")

  useEffect(() => {
    // fetch(props.url.backend + '/browser/' + props.user, {
    fetch(props.url.backend + location.pathname, {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({
        user: props.user
      })
    })
    .then(response => response.json())
    .then(data => browserValue[1]({
      type: "setFileList",
      fileList: key2list({
        files: data.files,
        levelMax: props.levelMax
      })
    }))
    .catch(error => {console.error('Error:', error);});
    // eslint-disable-next-line react-hooks/exhaustive-deps
  }, [props.user, props.url.backend, props.levelMax])

  return (
    <browserContext.Provider value={browserValue}>
      <div>{status}</div>
      {browserValue[0].fileList.map((file,i) => {
        if (!file.collapsed) {
          if (file.type === 'folder') {
            // console.log(file.key)
            return (<Folder {...props} setCurrent={props.setCurrent} setStatus={setStatus} key={i} keyValue={i} file={file}></Folder>)
          } else {
            if (props.showFiles) {
              return (<File {...props} setStatus={setStatus} key={i} keyValue={i} file={file}></File>)
            }
          }
        }
        return null;
      })}
    </browserContext.Provider>
  );
}


export default Browser;