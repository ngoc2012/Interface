
function getValues(obj) {
  let newObj = {}
  Object.entries(obj).forEach(([key, o]) => {
    if( o !== null && typeof o == "object" ) {
      if ('value' in o) {
        newObj[key] = o.value
      } else {
        newObj[key] = getValues(o)
      }
    }
  });
  return newObj;
}


export default getValues;