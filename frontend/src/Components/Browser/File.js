import React, {useState, useContext} from 'react'
import {browserContext} from './browserContext';
import Cookies from 'universal-cookie';
import {withRouter} from 'react-router-dom';
// you can use withRouter HOC from react-router to get the router props and call props.history.push(destUrlEdit)

import './Browser.css';
import Popup from './Popup';

function File(props) {

  const indent = "\u00a0\u00a0\u00a0\u00a0"
  const [browserValue, browserValueDispatch] = useContext(browserContext);
  const [editable, setEditable] = useState(false)
  const [filename, setFilename] = useState(props.file.name)
  const [size, setSize] = useState(props.file.name.length)
  const [showPopup, setShowPopup] = useState(false)

  const open = () => {
    let filename = props.file.key;
    const cookies = new Cookies();
    cookies.set('file', filename, { path: '/' });
    var win = window.open(props.url.frontend + '/editor/' + props.user + filename, '_blank');
    win.focus();
  }

  const onClickHandle = () => {
    if ((props.file.key) !== browserValue.activeFile) {
      browserValueDispatch({type: "setActiveFile", activeFile: props.file.key})
    }
  }

  const close = () => {browserValueDispatch({type: "setActiveFile", activeFile: ""})}

  const duplicate = () => {
    fetch(props.url.backend + '/projects/files', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({
        user: props.user,
        filename: props.file.key,
        type: 'duplicate'
      })
    })
    .then(response => response.json())
    .then(data => {
      let folders = data.newFileName.split("/")
      browserValueDispatch({
        type: "insertFile",
        key: props.keyValue+1,
        file: {
          ...props.file, 
          name: folders.slice(-1)[0],
          key: data.newFileName
        }
      })
      props.setStatus(data.status)
    })
    .catch(error => {console.error('Error:', error);});
  }

  const deleteFile = () => {
    fetch(props.url.backend + '/projects/files', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({
        user: props.user,
        filename: props.file.key,
        type: 'delete'
      })
    })
    .then(response => response.json())
    .then(data => {
      browserValueDispatch({
        type: "deleteFile",
        key: props.keyValue,
      })
      props.setStatus(data.status)
    })
    .catch(error => {console.error('Error:', error);});
  }

  // ================================================================
  // rename functions:
  const rename = () => {setEditable(true)}
  const handleOnchange = (e) => {
    let newname = props.file.key.split("/")
    newname[newname.length-1] = filename
    // console.log(e.target.value)
    fetch(props.url.backend + '/projects/files', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({
        user: props.user,
        filename: props.file.key,
        newname: newname.join('/'),
        type: 'rename'
      })
    })
    .then(response => response.json())
    .then(data => {
      browserValueDispatch({
        type: "renameFile",
        key: props.keyValue,
        file: {
          ...props.file, 
          name: filename,
          key: newname.join('/')
        }
      })
      props.setStatus(data.status)
      setEditable(false)
    })
    .catch(error => {console.error('Error:', error);});
  }

  const inputChange=(e) => {
    setFilename(e.target.value)
    setSize(e.target.value.length)
  }

  const onBlurHandle = () => {setEditable(false)}

  // ================================================================
  // movecopyto functions:
  const closePopup = () => {setShowPopup(false)}

  const selectFolder = () => {setShowPopup(true)}

  const movecopyto = (newfolder, action) => {
    fetch(props.url.backend + '/projects/files', {
      method: 'POST',
      headers: {'Content-Type': 'application/json'},
      body: JSON.stringify({
        user: props.user,
        filename: props.file.key,
        newfolder: newfolder,
        type: action
      })
    })
    .then(response => response.json())
    .then(data => {
      browserValueDispatch({
        type: action,
        key: props.keyValue,
        newfolder: newfolder,
        filekey: data.key,
        filename: data.newfilename
      })
      props.setStatus(data.status)
      let parentArray = newfolder.split('/')
      browserValueDispatch({type: 'expandFolder', parent: parentArray.slice(1,parentArray.length)})
    })
    .catch(error => {console.error('Error:', error);});
  }
  // console.log(props.file)
  return (
    <>
    {showPopup && <Popup {...props} close={closePopup} handle={movecopyto}></Popup>}
    <div 
      level={props.file.level} 
      className={(props.file.key) === browserValue.activeFile ? "fileSelected" : "fileNotSelected"}
      onDoubleClick={open}
      onClick={onClickHandle}>
      
      {editable && (
        <form
          onSubmit={e => {
            e.preventDefault();
            handleOnchange(e)
          }}
        >
          <input 
            autoFocus
            type="text"
            value={filename} 
            onChange={inputChange}
            size={size}
            onBlur={onBlurHandle}
          ></input>
        </form>
      )}
      
      {!editable && (<div>{indent.repeat(props.file.level-1)}{props.file.name}&nbsp;[{props.file.modified}]</div>)}
      {(props.file.key) === browserValue.activeFile && (<>{indent}<button onClick={open}>Open</button><button onClick={rename}>Renommer</button><button onClick={duplicate}>Doubler</button><button onClick={selectFolder}>Déplacer/Copier vers</button><button onClick={deleteFile}>Supprimer</button><button onClick={close}>Fermer</button></>)}
    </div>
    </>
  )
}
export default withRouter(File);
