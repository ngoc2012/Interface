export default function browserReducer(state, action) {
  // console.log(action)
  switch (action.type) {
    case 'setActiveFile': return {...state, activeFile: action.activeFile};
    case 'setStatus': return {...state, status: action.status};
    case 'setFileList': return {...state, fileList: action.fileList};
    case 'insertFile': {
      let fileList = [...state.fileList]
      fileList.splice(action.key, 0, action.file)
      return {...state, fileList: fileList}
    };
    case 'deleteFile': {
      let fileList = [...state.fileList]
      fileList.splice(action.key, 1)
      return {...state, fileList: fileList}
    };
    case 'renameFile': {
      let fileList = [...state.fileList]
      fileList[action.key] = action.file
      return {...state, fileList: fileList}
    };
    case 'moveto': {
      // not performable
      function arraymove(arr, fromIndex, toIndex) {
        var element = arr[fromIndex];
        arr.splice(fromIndex, 1);
        arr.splice(toIndex, 0, element);
      };
      let fileList = [...state.fileList]
      let i = 0
      while (fileList[i].key !== action.newfolder && (i<(fileList.length-1))) {i+=1}
      arraymove(fileList, action.key, i+1)
      fileList[i+1] = {
        ...fileList[i+1],
        level: action.newfolder.split('/').length,
        name: action.filename,
        key: action.filekey
      }
      return {...state, fileList: fileList}
    };
    case 'copyto': {
      let fileList = [...state.fileList]
      let i = 0
      while (fileList[i].key !== action.newfolder && (i<(fileList.length-1))) {i+=1}
      fileList.splice(i+1, 0, {
        ...fileList[action.key],
        level: action.newfolder.split('/').length,
        name: action.filename,
        key: action.filekey
      })
      return {...state, fileList: fileList}
    };
    case 'collapseFolder': {
      let fileList = [...state.fileList]
      fileList.forEach(file => {
        if (file.parent.slice(0,action.parent.length).join('/') === action.parent.join('/')) {file.collapsed = true}
      })
      return {...state, fileList: fileList}
    };
    case 'expandFolder': {
      let fileList = [...state.fileList]
      fileList.forEach(file => {
        if (file.parent.join('/') === action.parent.join('/')) {file.collapsed = false}
      })
      return {...state, fileList: fileList}
    };
    default: throw new Error('Unexpected action');
  }
};