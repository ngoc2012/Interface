import React, {useContext} from 'react'
import {browserContext} from './browserContext';

import './Browser.css';

function Folder(props) {

  const indent = "\u00a0\u00a0\u00a0\u00a0"
  const [browserValue, browserValueDispatch] = useContext(browserContext);

  const folderDoubleClick = (e) => {
    let nextElement = browserValue.fileList[props.keyValue + 1]
    if (nextElement.parent[nextElement.parent.length-1] === props.file.name) {
      if (nextElement.collapsed) {
        browserValueDispatch({type: 'expandFolder', parent: nextElement.parent})
      } else {
        browserValueDispatch({type: 'collapseFolder', parent: nextElement.parent})
      }
    }
  }

  const onClickHandle = () => {
    if ((props.file.key) !== browserValue.activeFile) {
      browserValueDispatch({type: "setActiveFile", activeFile: props.file.key})
      props.setCurrent(props.file.key)
    }
  }

  return (
    <div
      level={props.file.level}
      className={(props.file.key) === browserValue.activeFile ? "fileSelected" : "fileNotSelected"}
      onDoubleClick={folderDoubleClick}
      onClick={onClickHandle}
    >
      {indent.repeat(props.file.level-1)}[{props.file.name}]
    </div>
  )
}

export default Folder;
