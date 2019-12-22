Page({
  data: {
    device_name: 32 ,
  },
  //调用onenetapi获取arduino测得数据
  onLoad: function (options) {
    var _this = this;
    //console.log(options.device_id);// 打印设备ID
    //console.log(app.data.api_key);// 打印API_KEY
    wx.request({
      url: 'https://api.heclouds.com/devices/562234373/datapoints',
      header: {
        'api-key': 'Rh3VomUuIy1yXEpJURmUlx7npRE='
      },
      onLoad: function (res) {
        console.log(res.data);
        _this.setData(
          {
            device_name: res.data.data.datastreams[0].datapoints[0].value,

          });//实时更新数据
      },
      fail: function () {
        wx.showToast({
          title: '与服务器通信失败',
          icon: 'fail',
          duration: 2000
        })
      }
    })
  },
  lookfor:function(){
    wx.navigateTo
   ({
      url: '../case/case',   //跳转界面
    })
  },
 
  switchpage: function () {
    wx.navigateTo({
      url: '../index/index',//跳转界面
    })
  },
  switchpage1: function () {
    wx.navigateTo({
      url: '../clean/clean',//跳转界面
    })
  },

})
